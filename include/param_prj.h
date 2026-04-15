/*
 * This file is part of the stm32-template project.
 *
 * Copyright (C) 2020 Johannes Huebner <dev@johanneshuebner.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* This file contains all parameters used in your project
 * See main.cpp on how to access them.
 * If a parameters unit is of format "0=Choice, 1=AnotherChoice" etc.
 * It will be displayed as a dropdown in the web interface
 * If it is a spot value, the decimal is translated to the name, i.e. 0 becomes "Choice"
 * If the enum values are powers of two, they will be displayed as flags, example
 * "0=None, 1=Flag1, 2=Flag2, 4=Flag3, 8=Flag4" and the value is 5.
 * It means that Flag1 and Flag3 are active -> Display "Flag1 | Flag3"
 *
 * Every parameter/value has a unique ID that must never change. This is used when loading parameters
 * from flash, so even across firmware versions saved parameters in flash can always be mapped
 * back to our list here. If a new value is added, it will receive its default value
 * because it will not be found in flash.
 * The unique ID is also used in the CAN module, to be able to recover the CAN map
 * no matter which firmware version saved it to flash.
 * Make sure to keep track of your ids and avoid duplicates. Also don't re-assign
 * IDs from deleted parameters because you will end up loading some random value
 * into your new parameter!
 * IDs are 16 bit, so 65535 is the maximum
 */

//Define a version string of your firmware here
#define VER v0.9.1-pre3

/* Entries must be ordered as follows:
   1. Saveable parameters (id != 0)
   2. Temporary parameters (id = 0)
   3. Display values
 */
//Next param id (increase when adding new parameter!): 12
//Next value Id: 2322
/*              category     name         unit       min     max     default id */
#define PARAM_LIST \
    PARAM_ENTRY(CAT_BMS,     bmstype,     TYPES,     0,      2,      0,     1   ) \
    PARAM_ENTRY(CAT_BMS,     numbmbs,     "",        1,      32,      1,      2   ) \
    PARAM_ENTRY(CAT_BMS,     balance,     OFFON,     0,      1,      0,      3   ) \
    PARAM_ENTRY(CAT_BMS,    BallVthres,     "mV",   2800,   4500,   3900,      10   ) \
    PARAM_ENTRY(CAT_BMS,     nomcap,      "Ah",      0,      1000,   100,    4   ) \
    PARAM_ENTRY(CAT_BMS,    CellVmax,     "mV",      3000,  4200,   4150,     5   ) \
    PARAM_ENTRY(CAT_BMS,    CellVmin,     "mV",      2800,  3500,   3200,     6   ) \
    PARAM_ENTRY(CAT_BMS,     VmInterval,   "S",      1.5,    1000,   10,     12   ) \
    PARAM_ENTRY(CAT_SENS,    idcgain,     "dig/A",   -1000,  1000,   10,     7  ) \
    PARAM_ENTRY(CAT_SENS,    idcofs,      "dig",    -4095,   4095,   0,      8   ) \
    PARAM_ENTRY(CAT_SENS,    idcmode,     IDCMODES,  0,      3,      0,      9  ) \
    VALUE_ENTRY(opmode,      OPMODES,2000 ) \
    VALUE_ENTRY(version,     VERSTR, 2001 ) \
    VALUE_ENTRY(soc,         "%",   2002 ) \
    VALUE_ENTRY(chargelim,   "A",    2003 ) \
    VALUE_ENTRY(dischargelim,"A",    2004 ) \
    VALUE_ENTRY(chargeVlim,  "V",    2157 ) \
    VALUE_ENTRY(dischargeVlim,"V",   2158 ) \
    VALUE_ENTRY(deltaV,      "mV",   2005 ) \
    VALUE_ENTRY(udc,         "V",    2006 ) \
    VALUE_ENTRY(idc,         "A",    2007 ) \
    VALUE_ENTRY(IDCTrace,    "A",    2175 ) \
    VALUE_ENTRY(AsDiffTrace, "As",   2176 ) \
    VALUE_ENTRY(DiagUdc,     "V",    2301 ) \
    VALUE_ENTRY(DiagU1,      "mV",   2302 ) \
    VALUE_ENTRY(u1Age,       "s",    2195 ) \
    VALUE_ENTRY(DbgU1Raw,    "mV",   2291 ) \
    VALUE_ENTRY(DbgU1Fresh,  "",     2292 ) \
    VALUE_ENTRY(DbgU1SrcChip,"",     2303 ) \
    VALUE_ENTRY(DbgU1SrcSlot,"",     2304 ) \
    VALUE_ENTRY(DbgU1UpdCnt, "",     2305 ) \
    VALUE_ENTRY(DiagU12,     "mV",   2306 ) \
    VALUE_ENTRY(u12Age,      "s",    2206 ) \
    VALUE_ENTRY(DbgU12Raw,   "mV",   2293 ) \
    VALUE_ENTRY(DbgU12Fresh, "",     2294 ) \
    VALUE_ENTRY(DbgU12SrcChip,"",    2307 ) \
    VALUE_ENTRY(DbgU12SrcSlot,"",    2308 ) \
    VALUE_ENTRY(DbgU12UpdCnt,"",     2309 ) \
    VALUE_ENTRY(DiagU13,     "mV",   2310 ) \
    VALUE_ENTRY(u13Age,      "s",    2207 ) \
    VALUE_ENTRY(DbgU13Raw,   "mV",   2295 ) \
    VALUE_ENTRY(DbgU13Fresh, "",     2296 ) \
    VALUE_ENTRY(DbgU13SrcChip,"",    2311 ) \
    VALUE_ENTRY(DbgU13SrcSlot,"",    2312 ) \
    VALUE_ENTRY(DbgU13UpdCnt,"",     2313 ) \
    VALUE_ENTRY(DiagU86,     "mV",   2314 ) \
    VALUE_ENTRY(u86Age,      "s",    2280 ) \
    VALUE_ENTRY(DbgU86Raw,   "mV",   2297 ) \
    VALUE_ENTRY(DbgU86Fresh, "",     2298 ) \
    VALUE_ENTRY(DbgU86SrcChip,"",    2315 ) \
    VALUE_ENTRY(DbgU86SrcSlot,"",    2316 ) \
    VALUE_ENTRY(DbgU86UpdCnt,"",     2317 ) \
    VALUE_ENTRY(DiagU96,     "mV",   2318 ) \
    VALUE_ENTRY(u96Age,      "s",    2290 ) \
    VALUE_ENTRY(DbgU96Raw,   "mV",   2299 ) \
    VALUE_ENTRY(DbgU96Fresh, "",     2300 ) \
    VALUE_ENTRY(DbgU96SrcChip,"",    2319 ) \
    VALUE_ENTRY(DbgU96SrcSlot,"",    2320 ) \
    VALUE_ENTRY(DbgU96UpdCnt,"",     2321 ) \
    VALUE_ENTRY(CellStaleCount,"",    2193 ) \
    VALUE_ENTRY(CellMaxAge,   "s",    2194 ) \
    VALUE_ENTRY(CellGrp0Fresh,"",    2177 ) \
    VALUE_ENTRY(CellGrp1Fresh,"",    2178 ) \
    VALUE_ENTRY(CellGrp2Fresh,"",    2179 ) \
    VALUE_ENTRY(CellGrp3Fresh,"",    2180 ) \
    VALUE_ENTRY(CellGrp4Fresh,"",    2181 ) \
    VALUE_ENTRY(CellDataStaleWarn,"", 2182 ) \
    VALUE_ENTRY(CellsPresent,  "",   2159 ) \
    VALUE_ENTRY(CellGrp0First,"",    2183 ) \
    VALUE_ENTRY(CellGrp0Last,"",     2184 ) \
    VALUE_ENTRY(CellGrp1First,"",    2185 ) \
    VALUE_ENTRY(CellGrp1Last,"",     2186 ) \
    VALUE_ENTRY(CellGrp2First,"",    2187 ) \
    VALUE_ENTRY(CellGrp2Last,"",     2188 ) \
    VALUE_ENTRY(CellGrp3First,"",    2189 ) \
    VALUE_ENTRY(CellGrp3Last,"",     2190 ) \
    VALUE_ENTRY(CellGrp4First,"",    2191 ) \
    VALUE_ENTRY(CellGrp4Last,"",     2192 ) \
    VALUE_ENTRY(TempMax,     "°C",   2008 ) \
    VALUE_ENTRY(TempMin,     "°C",   2156 ) \
    VALUE_ENTRY(uavg,        "mV",   2009 ) \
    VALUE_ENTRY(umin,        "mV",   2010 ) \
    VALUE_ENTRY(CellMin,       "",   2169 ) \
    VALUE_ENTRY(umax,        "mV",   2011 ) \
    VALUE_ENTRY(CellMax,       "",   2170 ) \
    VALUE_ENTRY(u1,          "mV",   2012 ) \
    VALUE_ENTRY(u2,          "mV",   2013 ) \
    VALUE_ENTRY(u3,          "mV",   2014 ) \
    VALUE_ENTRY(u4,          "mV",   2015 ) \
    VALUE_ENTRY(u5,          "mV",   2016 ) \
    VALUE_ENTRY(u6,          "mV",   2017 ) \
    VALUE_ENTRY(u7,          "mV",   2018 ) \
    VALUE_ENTRY(u8,          "mV",   2019 ) \
    VALUE_ENTRY(u9,          "mV",   2020 ) \
    VALUE_ENTRY(u10,         "mV",   2021 ) \
    VALUE_ENTRY(u11,         "mV",   2022 ) \
    VALUE_ENTRY(u12,         "mV",   2023 ) \
    VALUE_ENTRY(u13,         "mV",   2024 ) \
    VALUE_ENTRY(u14,         "mV",   2025 ) \
    VALUE_ENTRY(u15,         "mV",   2026 ) \
    VALUE_ENTRY(u16,         "mV",   2027 ) \
    VALUE_ENTRY(u17,         "mV",   2028 ) \
    VALUE_ENTRY(u18,         "mV",   2029 ) \
    VALUE_ENTRY(u19,         "mV",   2030 ) \
    VALUE_ENTRY(u20,         "mV",   2031 ) \
    VALUE_ENTRY(u21,         "mV",   2032 ) \
    VALUE_ENTRY(u22,         "mV",   2033 ) \
    VALUE_ENTRY(u23,         "mV",   2035 ) \
    VALUE_ENTRY(u24,         "mV",   2036 ) \
    VALUE_ENTRY(u25,         "mV",   2037 ) \
    VALUE_ENTRY(u26,         "mV",   2038 ) \
    VALUE_ENTRY(u27,         "mV",   2039 ) \
    VALUE_ENTRY(u28,         "mV",   2040 ) \
    VALUE_ENTRY(u29,         "mV",   2041 ) \
    VALUE_ENTRY(u30,         "mV",   2042 ) \
    VALUE_ENTRY(u31,         "mV",   2043 ) \
    VALUE_ENTRY(u32,         "mV",   2044 ) \
    VALUE_ENTRY(u33,         "mV",   2045 ) \
    VALUE_ENTRY(u34,         "mV",   2046 ) \
    VALUE_ENTRY(u35,         "mV",   2047 ) \
    VALUE_ENTRY(u36,         "mV",   2048 ) \
    VALUE_ENTRY(u37,         "mV",   2049 ) \
    VALUE_ENTRY(u38,         "mV",   2050 ) \
    VALUE_ENTRY(u39,         "mV",   2051 ) \
    VALUE_ENTRY(u40,         "mV",   2052 ) \
    VALUE_ENTRY(u41,         "mV",   2053 ) \
    VALUE_ENTRY(u42,         "mV",   2054 ) \
    VALUE_ENTRY(u43,         "mV",   2055 ) \
    VALUE_ENTRY(u44,         "mV",   2056 ) \
    VALUE_ENTRY(u45,         "mV",   2057 ) \
    VALUE_ENTRY(u46,         "mV",   2058 ) \
    VALUE_ENTRY(u47,         "mV",   2059 ) \
    VALUE_ENTRY(u48,         "mV",   2060 ) \
    VALUE_ENTRY(u49,         "mV",   2061 ) \
    VALUE_ENTRY(u50,         "mV",   2062 ) \
    VALUE_ENTRY(u51,         "mV",   2063 ) \
    VALUE_ENTRY(u52,         "mV",   2064 ) \
    VALUE_ENTRY(u53,         "mV",   2065 ) \
    VALUE_ENTRY(u54,         "mV",   2066 ) \
    VALUE_ENTRY(u55,         "mV",   2067 ) \
    VALUE_ENTRY(u56,         "mV",   2068 ) \
    VALUE_ENTRY(u57,         "mV",   2069 ) \
    VALUE_ENTRY(u58,         "mV",   2070 ) \
    VALUE_ENTRY(u59,         "mV",   2071 ) \
    VALUE_ENTRY(u60,         "mV",   2072 ) \
    VALUE_ENTRY(u61,         "mV",   2073 ) \
    VALUE_ENTRY(u62,         "mV",   2074 ) \
    VALUE_ENTRY(u63,         "mV",   2075 ) \
    VALUE_ENTRY(u64,         "mV",   2076 ) \
    VALUE_ENTRY(u65,         "mV",   2077 ) \
    VALUE_ENTRY(u66,         "mV",   2078 ) \
    VALUE_ENTRY(u67,         "mV",   2079 ) \
    VALUE_ENTRY(u68,         "mV",   2080 ) \
    VALUE_ENTRY(u69,         "mV",   2081 ) \
    VALUE_ENTRY(u70,         "mV",   2082 ) \
    VALUE_ENTRY(u71,         "mV",   2083 ) \
    VALUE_ENTRY(u72,         "mV",   2084 ) \
    VALUE_ENTRY(u73,         "mV",   2085 ) \
    VALUE_ENTRY(u74,         "mV",   2086 ) \
    VALUE_ENTRY(u75,         "mV",   2087 ) \
    VALUE_ENTRY(u76,         "mV",   2088 ) \
    VALUE_ENTRY(u77,         "mV",   2089 ) \
    VALUE_ENTRY(u78,         "mV",   2090 ) \
    VALUE_ENTRY(u79,         "mV",   2091 ) \
    VALUE_ENTRY(u80,         "mV",   2092 ) \
    VALUE_ENTRY(u81,         "mV",   2093 ) \
    VALUE_ENTRY(u82,         "mV",   2094 ) \
    VALUE_ENTRY(u83,         "mV",   2095 ) \
    VALUE_ENTRY(u84,         "mV",   2096 ) \
    VALUE_ENTRY(u85,         "mV",   2097 ) \
    VALUE_ENTRY(u86,         "mV",   2098 ) \
    VALUE_ENTRY(u87,         "mV",   2099 ) \
    VALUE_ENTRY(u88,         "mV",   2100 ) \
    VALUE_ENTRY(u89,         "mV",   2101 ) \
    VALUE_ENTRY(u90,         "mV",   2102 ) \
    VALUE_ENTRY(u91,         "mV",   2103 ) \
    VALUE_ENTRY(u92,         "mV",   2104 ) \
    VALUE_ENTRY(u93,         "mV",   2105 ) \
    VALUE_ENTRY(u94,         "mV",   2106 ) \
    VALUE_ENTRY(u95,         "mV",   2107 ) \
    VALUE_ENTRY(u96,         "mV",   2108 ) \
    VALUE_ENTRY(u97,         "mV",   2109 ) \
    VALUE_ENTRY(u98,         "mV",   2110 ) \
    VALUE_ENTRY(u99,         "mV",   2111 ) \
    VALUE_ENTRY(u100,        "mV",   2112 ) \
    VALUE_ENTRY(u101,        "mV",   2113 ) \
    VALUE_ENTRY(u102,        "mV",   2114 ) \
    VALUE_ENTRY(u103,        "mV",   2115 ) \
    VALUE_ENTRY(u104,        "mV",   2116 ) \
    VALUE_ENTRY(u105,        "mV",   2117 ) \
    VALUE_ENTRY(u106,        "mV",   2118 ) \
    VALUE_ENTRY(u107,        "mV",   2119 ) \
    VALUE_ENTRY(u108,        "mV",   2120 ) \
    VALUE_ENTRY(Cellt0_0,    "°C",   2140 ) \
    VALUE_ENTRY(Cellt0_1,    "°C",   2141 ) \
    VALUE_ENTRY(Cellt1_0,    "°C",   2142 ) \
    VALUE_ENTRY(Cellt1_1,    "°C",   2143 ) \
    VALUE_ENTRY(Cellt2_0,    "°C",   2144 ) \
    VALUE_ENTRY(Cellt2_1,    "°C",   2145 ) \
    VALUE_ENTRY(Cellt3_0,    "°C",   2146 ) \
    VALUE_ENTRY(Cellt3_1,    "°C",   2147 ) \
    VALUE_ENTRY(Cellt4_0,    "°C",   2148 ) \
    VALUE_ENTRY(Cellt4_1,    "°C",   2149 ) \
    VALUE_ENTRY(Cellt5_0,    "°C",   2150 ) \
    VALUE_ENTRY(Cellt5_1,    "°C",   2151 ) \
    VALUE_ENTRY(Cellt6_0,    "°C",   2152 ) \
    VALUE_ENTRY(Cellt6_1,    "°C",   2153 ) \
    VALUE_ENTRY(Cellt7_0,    "°C",   2154 ) \
    VALUE_ENTRY(Cellt7_1,    "°C",   2155 ) \
    VALUE_ENTRY(Chipt0,      "°C",   2121 ) \
    VALUE_ENTRY(Chipt1,      "°C",   2122 ) \
    VALUE_ENTRY(Chipt2,      "°C",   2123 ) \
    VALUE_ENTRY(Chipt3,      "°C",   2124 ) \
    VALUE_ENTRY(Chipt4,      "°C",   2125 ) \
    VALUE_ENTRY(Chipt5,      "°C",   2126 ) \
    VALUE_ENTRY(Chipt6,      "°C",   2127 ) \
    VALUE_ENTRY(Chipt7,      "°C",   2128 ) \
    VALUE_ENTRY(Chip1_5V,    "mV",   2138 ) \
    VALUE_ENTRY(Chip2_5V,    "mV",   2139 ) \
    VALUE_ENTRY(ChipV1,       "V",   2130 ) \
    VALUE_ENTRY(ChipV2,       "V",   2131 ) \
    VALUE_ENTRY(ChipV3,       "V",   2132 ) \
    VALUE_ENTRY(ChipV4,       "V",   2133 ) \
    VALUE_ENTRY(ChipV5,       "V",   2134 ) \
    VALUE_ENTRY(ChipV6,       "V",   2135 ) \
    VALUE_ENTRY(ChipV7,       "V",   2136 ) \
    VALUE_ENTRY(ChipV8,       "V",   2137 ) \
    VALUE_ENTRY(Chip1Cells,    "",   2161 ) \
    VALUE_ENTRY(Chip2Cells,    "",   2162 ) \
    VALUE_ENTRY(Chip3Cells,    "",   2163 ) \
    VALUE_ENTRY(Chip4Cells,    "",   2164 ) \
    VALUE_ENTRY(Chip5Cells,    "",   2165 ) \
    VALUE_ENTRY(Chip6Cells,    "",   2166 ) \
    VALUE_ENTRY(Chip7Cells,    "",   2167 ) \
    VALUE_ENTRY(Chip8Cells,    "",   2168 ) \
    VALUE_ENTRY(CellsBalancing,"",   2160 ) \
    VALUE_ENTRY(CellsBalancingCmd,"", 2174 ) \
    VALUE_ENTRY(LoopCnt,      "",    2171 ) \
    VALUE_ENTRY(LoopState,    "",    2172 ) \
    VALUE_ENTRY(PecErrCnt,    "",    2173 ) \
    VALUE_ENTRY(cpuload,     "%",    2129 ) \
    VALUE_ENTRY(u2Age,        "s",   2196 ) \
    VALUE_ENTRY(u3Age,        "s",   2197 ) \
    VALUE_ENTRY(u4Age,        "s",   2198 ) \
    VALUE_ENTRY(u5Age,        "s",   2199 ) \
    VALUE_ENTRY(u6Age,        "s",   2200 ) \
    VALUE_ENTRY(u7Age,        "s",   2201 ) \
    VALUE_ENTRY(u8Age,        "s",   2202 ) \
    VALUE_ENTRY(u9Age,        "s",   2203 ) \
    VALUE_ENTRY(u10Age,       "s",   2204 ) \
    VALUE_ENTRY(u11Age,       "s",   2205 ) \
    VALUE_ENTRY(u14Age,       "s",   2208 ) \
    VALUE_ENTRY(u15Age,       "s",   2209 ) \
    VALUE_ENTRY(u16Age,       "s",   2210 ) \
    VALUE_ENTRY(u17Age,       "s",   2211 ) \
    VALUE_ENTRY(u18Age,       "s",   2212 ) \
    VALUE_ENTRY(u19Age,       "s",   2213 ) \
    VALUE_ENTRY(u20Age,       "s",   2214 ) \
    VALUE_ENTRY(u21Age,       "s",   2215 ) \
    VALUE_ENTRY(u22Age,       "s",   2216 ) \
    VALUE_ENTRY(u23Age,       "s",   2217 ) \
    VALUE_ENTRY(u24Age,       "s",   2218 ) \
    VALUE_ENTRY(u25Age,       "s",   2219 ) \
    VALUE_ENTRY(u26Age,       "s",   2220 ) \
    VALUE_ENTRY(u27Age,       "s",   2221 ) \
    VALUE_ENTRY(u28Age,       "s",   2222 ) \
    VALUE_ENTRY(u29Age,       "s",   2223 ) \
    VALUE_ENTRY(u30Age,       "s",   2224 ) \
    VALUE_ENTRY(u31Age,       "s",   2225 ) \
    VALUE_ENTRY(u32Age,       "s",   2226 ) \
    VALUE_ENTRY(u33Age,       "s",   2227 ) \
    VALUE_ENTRY(u34Age,       "s",   2228 ) \
    VALUE_ENTRY(u35Age,       "s",   2229 ) \
    VALUE_ENTRY(u36Age,       "s",   2230 ) \
    VALUE_ENTRY(u37Age,       "s",   2231 ) \
    VALUE_ENTRY(u38Age,       "s",   2232 ) \
    VALUE_ENTRY(u39Age,       "s",   2233 ) \
    VALUE_ENTRY(u40Age,       "s",   2234 ) \
    VALUE_ENTRY(u41Age,       "s",   2235 ) \
    VALUE_ENTRY(u42Age,       "s",   2236 ) \
    VALUE_ENTRY(u43Age,       "s",   2237 ) \
    VALUE_ENTRY(u44Age,       "s",   2238 ) \
    VALUE_ENTRY(u45Age,       "s",   2239 ) \
    VALUE_ENTRY(u46Age,       "s",   2240 ) \
    VALUE_ENTRY(u47Age,       "s",   2241 ) \
    VALUE_ENTRY(u48Age,       "s",   2242 ) \
    VALUE_ENTRY(u49Age,       "s",   2243 ) \
    VALUE_ENTRY(u50Age,       "s",   2244 ) \
    VALUE_ENTRY(u51Age,       "s",   2245 ) \
    VALUE_ENTRY(u52Age,       "s",   2246 ) \
    VALUE_ENTRY(u53Age,       "s",   2247 ) \
    VALUE_ENTRY(u54Age,       "s",   2248 ) \
    VALUE_ENTRY(u55Age,       "s",   2249 ) \
    VALUE_ENTRY(u56Age,       "s",   2250 ) \
    VALUE_ENTRY(u57Age,       "s",   2251 ) \
    VALUE_ENTRY(u58Age,       "s",   2252 ) \
    VALUE_ENTRY(u59Age,       "s",   2253 ) \
    VALUE_ENTRY(u60Age,       "s",   2254 ) \
    VALUE_ENTRY(u61Age,       "s",   2255 ) \
    VALUE_ENTRY(u62Age,       "s",   2256 ) \
    VALUE_ENTRY(u63Age,       "s",   2257 ) \
    VALUE_ENTRY(u64Age,       "s",   2258 ) \
    VALUE_ENTRY(u65Age,       "s",   2259 ) \
    VALUE_ENTRY(u66Age,       "s",   2260 ) \
    VALUE_ENTRY(u67Age,       "s",   2261 ) \
    VALUE_ENTRY(u68Age,       "s",   2262 ) \
    VALUE_ENTRY(u69Age,       "s",   2263 ) \
    VALUE_ENTRY(u70Age,       "s",   2264 ) \
    VALUE_ENTRY(u71Age,       "s",   2265 ) \
    VALUE_ENTRY(u72Age,       "s",   2266 ) \
    VALUE_ENTRY(u73Age,       "s",   2267 ) \
    VALUE_ENTRY(u74Age,       "s",   2268 ) \
    VALUE_ENTRY(u75Age,       "s",   2269 ) \
    VALUE_ENTRY(u76Age,       "s",   2270 ) \
    VALUE_ENTRY(u77Age,       "s",   2271 ) \
    VALUE_ENTRY(u78Age,       "s",   2272 ) \
    VALUE_ENTRY(u79Age,       "s",   2273 ) \
    VALUE_ENTRY(u80Age,       "s",   2274 ) \
    VALUE_ENTRY(u81Age,       "s",   2275 ) \
    VALUE_ENTRY(u82Age,       "s",   2276 ) \
    VALUE_ENTRY(u83Age,       "s",   2277 ) \
    VALUE_ENTRY(u84Age,       "s",   2278 ) \
    VALUE_ENTRY(u85Age,       "s",   2279 ) \
    VALUE_ENTRY(u87Age,       "s",   2281 ) \
    VALUE_ENTRY(u88Age,       "s",   2282 ) \
    VALUE_ENTRY(u89Age,       "s",   2283 ) \
    VALUE_ENTRY(u90Age,       "s",   2284 ) \
    VALUE_ENTRY(u91Age,       "s",   2285 ) \
    VALUE_ENTRY(u92Age,       "s",   2286 ) \
    VALUE_ENTRY(u93Age,       "s",   2287 ) \
    VALUE_ENTRY(u94Age,       "s",   2288 ) \
    VALUE_ENTRY(u95Age,       "s",   2289 )


/***** Enum String definitions *****/
#define OPMODES      "0=Off, 1=Run, 2=RunBalance"
#define OFFON        "0=Off, 1=On"
#define BAL          "0=None, 1=Discharge"
#define IDCMODES     "0=Off, 1=AdcSingle, 2=IsaCan"
#define TYPES        "0=Model_3, 1=Model_S, 2=MAX"
#define CAT_BMS      "BMS"
#define CAT_SENS     "Sensor setup"
#define CAT_COMM     "Communication"

#define VERSTR STRINGIFY(4=VER)

/***** enums ******/

enum
{
    IDC_OFF, IDC_SINGLE, IDC_ISACAN
};


enum _modes
{
    MOD_OFF = 0,
    MOD_RUN,
    MOD_LAST
};

enum _types
{
    BMS_M3 = 0,
    BMS_TESLAS = 1,
    BMS_MAX = 2
};

//Generated enum-string for possible errors
extern const char* errorListString;
