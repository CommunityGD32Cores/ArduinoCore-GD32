# Copyright 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Extended and rewritten by Maximilian Gerhardt <maximilian.gerhardt@rub.de>
# for GD32 core.

"""
Arduino

Arduino Wiring-based Framework allows writing cross-platform software to
control devices attached to a wide range of Arduino boards to create all
kinds of creative coding, interactive objects, spaces or physical experiences.

https://github.com/CommunityGD32Cores/GD32Core-New
"""

import json
from os.path import isfile, isdir, join

from platformio.util import get_systype

from SCons.Script import COMMAND_LINE_TARGETS, DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()
board_config = env.BoardConfig()
board_name = env.subst("$BOARD")

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinogd32")
assert isdir(FRAMEWORK_DIR)


mcu = board_config.get("build.mcu", "")
mcu_type = mcu[:-2]
variant = board_config.get(
    "build.variant", board_config.get("build.arduino.variant", "generic"))
series = mcu_type[:6].upper() + "xx"
variants_dir = (
    join("$PROJECT_DIR", board_config.get("build.variants_dir"))
    if board_config.get("build.variants_dir", "")
    else join(FRAMEWORK_DIR, "variants")
)
variant_dir = join(variants_dir, variant)
inc_variant_dir = variant_dir
if "windows" not in get_systype().lower() and not (
    set(["_idedata", "idedata"]) & set(COMMAND_LINE_TARGETS) and " " not in variant_dir
):
    inc_variant_dir = variant_dir.replace("(", r"\(").replace(")", r"\)")

upload_protocol = env.subst("$UPLOAD_PROTOCOL")


def process_standard_library_configuration(cpp_defines):
    if "PIO_FRAMEWORK_ARDUINO_STANDARD_LIB" in cpp_defines:
        env["LINKFLAGS"].remove("--specs=nano.specs")
    if "PIO_FRAMEWORK_ARDUINO_NANOLIB_FLOAT_PRINTF" in cpp_defines:
        env.Append(LINKFLAGS=["-u_printf_float"])
    if "PIO_FRAMEWORK_ARDUINO_NANOLIB_FLOAT_SCANF" in cpp_defines:
        env.Append(LINKFLAGS=["-u_scanf_float"])


def process_usart_configuration(cpp_defines):
    if "PIO_FRAMEWORK_ARDUINO_SERIAL_DISABLED" in cpp_defines:
        env["CPPDEFINES"].remove("HAL_UART_MODULE_ENABLED")

    elif "PIO_FRAMEWORK_ARDUINO_SERIAL_WITHOUT_GENERIC" in cpp_defines:
        env.Append(CPPDEFINES=["HWSERIAL_NONE"])

def add_upload_protocol_defines(board, upload_protocol):
    if upload_protocol == "serial":
        env.Append(
            CPPDEFINES=[("CONFIG_MAPLE_MINI_NO_DISABLE_DEBUG", 1)])
    elif upload_protocol == "dfu":
        env.Append(CPPDEFINES=["SERIAL_USB"])
    else:
        env.Append(
            CPPDEFINES=[
                ("CONFIG_MAPLE_MINI_NO_DISABLE_DEBUG", 1),
                "SERIAL_USB"
            ])

    is_generic = board.startswith("generic") or board == "hytiny_stm32f103t"
    if upload_protocol in ("stlink", "dfu", "jlink") and is_generic:
        env.Append(CPPDEFINES=["GENERIC_BOOTLOADER"])


def get_arm_math_lib(cpu):
    core = board_config.get("build.cpu")
    if "m33" in core:
        return "arm_ARMv8MMLlfsp_math"
    elif "m4" in core:
        return "arm_cortexM4lf_math"
    elif "m7" in core:
        return "arm_cortexM7lfsp_math"

    return "arm_cortex%sl_math" % core[7:9].upper()


def configure_application_offset(mcu, upload_protocol):
    offset = 0

    if upload_protocol == "hid":
        if mcu.startswith("gd32f1"):
            offset = 0x800
        elif mcu.startswith("gd32f4"):
            offset = 0x4000

        env.Append(CPPDEFINES=["BL_HID"])

    elif upload_protocol == "dfu":
        # GD32F103 series doesn't have embedded DFU over USB
        # stm32duino bootloader (v1, v2) is used instead
        if mcu.startswith("gd32f103") or mcu.startswith("gd32f303"):
            if board_config.get("upload.boot_version", 2) == 1:
                offset = 0x5000
            else:
                offset = 0x2000
            env.Append(CPPDEFINES=["BL_LEGACY_LEAF"])

    if offset != 0:
        env.Append(CPPDEFINES=[("VECT_TAB_OFFSET", "%s" % hex(offset))],)

    # LD_FLASH_OFFSET is mandatory even if there is no offset
    env.Append(LINKFLAGS=["-Wl,--defsym=LD_FLASH_OFFSET=%s" % hex(offset)])


if any(mcu in board_config.get("build.cpu") for mcu in ("cortex-m4", "cortex-m7")):
    env.Append(
        CCFLAGS=["-mfpu=fpv4-sp-d16", "-mfloat-abi=hard"],
        LINKFLAGS=["-mfpu=fpv4-sp-d16", "-mfloat-abi=hard"],
    )
if board_config.get("build.cpu") == "cortex-m33":
    env.Append(
        CCFLAGS=["-mfpu=fp-armv8", "-mfloat-abi=softfp"],
        LINKFLAGS=["-mfpu=fp-armv8", "-mfloat-abi=softfp"],
    )


def load_boards_remap():
    remap_file = join(FRAMEWORK_DIR, "tools", "platformio", "boards_remap.json")
    if not isfile(remap_file):
        print("Warning! Couldn't find board remap file!")
        return {}

    with open(remap_file, "r") as fp:
        try:
            return json.load(fp)
        except:
            print("Warning! Failed to parse board remap file!")
            return {}


def get_arduino_board_id(board_config, mcu):
    # User-specified value
    if board_config.get("build.arduino.board", ""):
        return board_config.get("build.arduino.board")

    # Default boards
    boards_remap = load_boards_remap()
    board_id = env.subst("$BOARD")
    if board_id in boards_remap:
        return boards_remap[board_id]

    # Fall back to default cases according to MCU value for generic boards
    if board_id.lower().startswith("generic"):
        board_id = "GENERIC_"
        mcu = mcu.upper()
        if len(mcu) > 11:
            board_id += mcu[4:11] + "X"
        else:
            if len(mcu) > 10:
                board_id += mcu[4:10] + "TX"
            else:
                board_id += mcu
            print(
                "Warning! Couldn't generate proper internal board id from the `%s` MCU "
                "field! At least 12 symbols are required!" % mcu
            )

            print("Falling back to `%s`." % board_id)

    return board_id.upper()


board_id = get_arduino_board_id(board_config, mcu)
spl_series = board_config.get("build.spl_series","")

env.Append(
    ASFLAGS=["-x", "assembler-with-cpp"],
    CFLAGS=["-std=gnu11"],
    CXXFLAGS=[
        "-std=gnu++14",
        "-fno-threadsafe-statics",
        "-fno-rtti",
        "-fno-exceptions",
        "-fno-use-cxa-atexit",
    ],
    CCFLAGS=[
        "-Os",  # optimize for size
        "-mcpu=%s" % board_config.get("build.cpu"),
        "-mthumb",
        "-ffunction-sections",  # place each function in its own section
        "-fdata-sections",
        "-nostdlib",
        "--param",
        "max-inline-insns-single=500",
        "-masm-syntax-unified",
    ],
    CPPDEFINES=[
        series,
        ("ARDUINO", 10808),
        ("F_CPU", "$BOARD_F_CPU"), # for compatiblity
        "ARDUINO_ARCH_GD32",
        "ARDUINO_%s" % board_id,
        ("BOARD_NAME", '\\"%s\\"' % board_id),
        ("ARDUINO_UPLOAD_MAXIMUM_SIZE", board_config.get("upload.maximum_size")),
    ],
    CPPPATH=[
        join(FRAMEWORK_DIR, "cores", "arduino", "api", "deprecated"),
        join(FRAMEWORK_DIR, "cores", "arduino", "api", "deprecated-avr-comp"),
        join(FRAMEWORK_DIR, "cores", "arduino", "gd32"),
        join(FRAMEWORK_DIR, "cores", "arduino", "gd32", "Source"),
        join(FRAMEWORK_DIR, "system", "startup"),
        join(FRAMEWORK_DIR, "system", spl_series + "_firmware", "CMSIS"),
        join(FRAMEWORK_DIR, "system", spl_series + "_firmware", "CMSIS", "GD", spl_series, "Include"),
        join(FRAMEWORK_DIR, "system", spl_series + "_firmware", "CMSIS", "GD", spl_series, "Source"),
        join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_standard_peripheral", "Include"),
        join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_standard_peripheral", "Source"),
        # USB libraries are included conditionally later
        join(FRAMEWORK_DIR, "cores", "arduino"),
    ],
    LINKFLAGS=[
        "-Os",
        "-mthumb",
        "-mcpu=%s" % board_config.get("build.cpu"),
        "--specs=nano.specs",
        "-Wl,--gc-sections,--relax",
        "-Wl,--check-sections",
        "-Wl,--entry=Reset_Handler",
        "-Wl,--unresolved-symbols=report-all",
        "-Wl,--warn-common",
        "-Wl,--defsym=LD_MAX_SIZE=%d" % board_config.get("upload.maximum_size"),
        "-Wl,--defsym=LD_MAX_DATA_SIZE=%d" % board_config.get("upload.maximum_ram_size"),
    ],
    LIBS=[
        #get_arm_math_lib(board_config.get("build.cpu")),  #math library not included
        "c",
        "m",
        "gcc",
        "stdc++",
    ],
#    LIBPATH=[join(CMSIS_DIR, "DSP", "Lib", "GCC")],
)

# For boards supporting a USB stack: Include it.
if not board_config.get("build.spl_series").lower().startswith("gd32e23"):
    if isdir(join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbd_library")):
        env.Append(
            CPPPATH=[
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbd_library", "device", "Include"),
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbd_library", "device", "Source"),
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbd_library", "usbd", "Include"),
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbd_library", "usbd", "Source"),
            ]
        )
    # never activate USB FS driver, although some have it -- we have no core support for it yet
    if isdir(join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbfs_driver")) and False:
        env.Append(
            CPPPATH=[
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbfs_driver", "driver", "Include"),
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbfs_driver", "driver", "Source"),
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbfs_driver", "core", "Include"),
                join(FRAMEWORK_DIR, "system", spl_series + "_firmware", spl_series + "_usbfs_driver", "core", "Source"),
            ]
        )

def process_usb_configuration(cpp_defines):
    # support standard way of enabling CDC
    if "PIO_FRAMEWORK_ARDUINO_ENABLE_CDC" in cpp_defines:
        env.Append(CPPDEFINES=["USBD_USE_CDC"])
    # any USB flags enabled? more might be to come 
    if any(
        d in cpp_defines
        for d in (
            "PIO_FRAMEWORK_ARDUINO_ENABLE_CDC",
        )
    ):
        # then add usb flags
        usb_vid = int(board_config.get("build.hwids", [["0xdead", "0xbeef"]])[0][0], 16)
        usb_pid = int(board_config.get("build.hwids", [["0xdead", "0xbeef"]])[0][1], 16)
        # prevent usage of Leaflabs VID/PID, otherwise if people have the DFU
        # driver installed, it will recognize it as "Maple DFU" and not the 
        # actual USB device it is :(
        if usb_vid == 0x1EAF and usb_pid == 0x0003:
            (usb_vid, usb_pid) = (0xdead, 0xbeef)
        env.Append(
            CPPDEFINES=[
                "USBCON",
                ("USB_VID", hex(usb_vid)),
                ("USB_PID", hex(usb_pid)),
                ("USB_PRODUCT", '\\"%s\\"' %
                    board_config.get("build.usb_product", board_config.get("name", "Undefined USB Product")).replace('"', "")),
                ("USB_MANUFACTURER", '\\"%s\\"' %
                    board_config.get("build.usb_manufacturer",  board_config.get("vendor", "Undefined Manufacturer")).replace('"', ""))
            ]
        )

env.ProcessFlags(board_config.get("build.framework_extra_flags.arduino", ""))

configure_application_offset(mcu, upload_protocol)

#
# Linker requires preprocessing with correct RAM|ROM sizes
#

if not board_config.get("build.ldscript", ""):
    #env.Replace(LDSCRIPT_PATH=join(FRAMEWORK_DIR, "system", "ldscript.ld"))
    if not isfile(join(env.subst(variant_dir), "ldscript.ld")):
        print("Warning! Cannot find linker script for the current target!\n")
    env.Append(
        LINKFLAGS=[
            (
                "-Wl,--default-script",
                join(
                    inc_variant_dir,
                    board_config.get("build.arduino.ldscript", "ldscript.ld"),
                ),
            )
        ]
    )

#
# Process configuration flags
#

cpp_defines = env.Flatten(env.get("CPPDEFINES", []))

process_standard_library_configuration(cpp_defines)
add_upload_protocol_defines(board_name, upload_protocol)
# defining HAL_UART_MODULE_ENABLED causes build failure 'uart_debug_write' was not declared in this scope
#process_usart_configuration(cpp_defines)
process_usb_configuration(cpp_defines)

# copy CCFLAGS to ASFLAGS (-x assembler-with-cpp mode)
env.Append(ASFLAGS=env.get("CCFLAGS", [])[:])

env.Append(
    LIBSOURCE_DIRS=[
        join(FRAMEWORK_DIR, "libraries"),
    ]
)

#
# Target: Build Core Library
#

libs = []

if "build.variant" in board_config:
    env.Append(
        CPPPATH=[inc_variant_dir],
        LIBPATH=[inc_variant_dir]
    )
    env.BuildSources(join("$BUILD_DIR", "FrameworkArduinoVariant"), variant_dir)

libs.append(env.BuildLibrary(
    join("$BUILD_DIR", "FrameworkArduino"), join(FRAMEWORK_DIR, "cores", "arduino")
))

env.Prepend(LIBS=libs)
