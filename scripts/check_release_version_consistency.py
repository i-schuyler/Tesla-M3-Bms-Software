#!/usr/bin/env python3

import argparse
import os
import re
import sys
from pathlib import Path


VER_DEFINE_PATTERN = re.compile(r"^\s*#define\s+VER\s+([^\s/]+)")
CHANGELOG_HEADING_PATTERN = re.compile(r"^##\s+(v[0-9A-Za-z][0-9A-Za-z.\-]*)\b")


def read_firmware_version(path: Path) -> tuple[str, int]:
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), start=1):
        match = VER_DEFINE_PATTERN.match(line)
        if match:
            return match.group(1), line_number

    raise ValueError(
        f"Could not find firmware version '#define VER ...' in {path}."
    )


def read_latest_changelog_version(path: Path) -> tuple[str, int]:
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), start=1):
        match = CHANGELOG_HEADING_PATTERN.match(line.strip())
        if match:
            return match.group(1), line_number

    raise ValueError(
        "Could not find a changelog release heading like '## vX.Y.Z' in "
        f"{path}."
    )


def normalize_tag(raw_tag: str) -> str:
    if raw_tag.startswith("refs/tags/"):
        return raw_tag[len("refs/tags/") :]
    return raw_tag


def resolve_tag(cli_tag: str | None) -> str | None:
    if cli_tag:
        return normalize_tag(cli_tag)

    github_ref = os.getenv("GITHUB_REF", "")
    if github_ref.startswith("refs/tags/"):
        github_ref_name = os.getenv("GITHUB_REF_NAME", "")
        return normalize_tag(github_ref_name or github_ref)

    return None


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Enforce firmware/changelog/tag version consistency in CI."
    )
    parser.add_argument(
        "--param-file",
        default="include/param_prj.h",
        help="Path to firmware parameter header (default: include/param_prj.h)",
    )
    parser.add_argument(
        "--changelog-file",
        default="docs/CHANGELOG.md",
        help="Path to changelog markdown (default: docs/CHANGELOG.md)",
    )
    parser.add_argument(
        "--require-changelog-match",
        action="store_true",
        help="Fail when latest changelog version differs from firmware version.",
    )
    parser.add_argument(
        "--tag",
        default=None,
        help="Tag name to enforce (for example: v1.2.3 or refs/tags/v1.2.3).",
    )
    args = parser.parse_args()

    param_file = Path(args.param_file)
    changelog_file = Path(args.changelog_file)

    try:
        firmware_version, firmware_line = read_firmware_version(param_file)
        changelog_version, changelog_line = read_latest_changelog_version(changelog_file)
    except FileNotFoundError as err:
        print(f"ERROR: Missing file: {err.filename}", file=sys.stderr)
        return 1
    except ValueError as err:
        print(f"ERROR: {err}", file=sys.stderr)
        return 1

    tag_name = resolve_tag(args.tag)

    print(
        f"[version-guard] Firmware version: {firmware_version} "
        f"({param_file}:{firmware_line})"
    )
    print(
        f"[version-guard] Changelog latest version: {changelog_version} "
        f"({changelog_file}:{changelog_line})"
    )
    if tag_name:
        print(f"[version-guard] Tag version: {tag_name}")

    errors: list[str] = []

    if args.require_changelog_match and firmware_version != changelog_version:
        errors.append(
            "Newest changelog version does not match firmware version: "
            f"{changelog_version} != {firmware_version}."
        )
    elif not args.require_changelog_match and firmware_version != changelog_version:
        print(
            "[version-guard] Changelog/version mismatch detected but not enforced "
            "for this run (docs/CHANGELOG.md not touched)."
        )

    if tag_name:
        if firmware_version != changelog_version:
            errors.append(
                "Tag run requires firmware and changelog versions to match: "
                f"{firmware_version} != {changelog_version}."
            )
        if tag_name != firmware_version:
            errors.append(
                f"Tag does not match firmware version: {tag_name} != {firmware_version}."
            )
        if tag_name != changelog_version:
            errors.append(
                f"Tag does not match changelog version: {tag_name} != {changelog_version}."
            )

    if errors:
        for message in errors:
            print(f"ERROR: {message}", file=sys.stderr)
        return 1

    print("[version-guard] Version consistency checks passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
