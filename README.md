# Getting started with the Mbed OS Key Value Store Global API

This example demonstrates how to use the Mbed OS Key Value Storage Global API.

KVStore is a key-value storage based API over a block device.
Mbed-OS provides several KVStore implementation options, that can be optimized to
the specific application requirements and target hardware:
- TDBStore - Default implementation of the KVStore API. It provides static wear-leveling and quick access for when you have a small number of KV pairs.
- FileSystemStore - Class that provides a key-value store API on top of a POSIX-like file system API.
- SecureStore - Class that provides encryption, authentication and rollback protection on top of the KVStore API. It requires two KVStore implementations, one that provides the storage for the KV pairs and one providing storage for the CMACs that protect KV pairs stored in the KVStore.

KVStore Global APIs use Mbed-OS [KVStore configuration](#kvstore-configuration) json file to define the selected KVStore implementation option of the above.

You can find more information about Mbed OS Key Value Store Design and other related items in the [kvstore design documentation](https://github.com/ARMmbed/mbed-os/tree/master/docs/design-documents/features/storage/KVStore/KVStore_design.md).

**Table of contents:**

1. [Scenario](#scenario)
2. [Hardware Requirements](#hardware-requirements)
3. [Usage](#usage)
   - [Import the example](#import-the-example)
   - [Compile the example](#compile-the-example)
   - [Run the example](#run-the-example)
   - [Troubleshooting](#troubleshooting)
4. [Default Global KVStore Configuration](#kvstore-configuration)

## Scenario

The example sets several key/value pairs, some are defined as 'dummy' and some as 'real'. The example demonstrates several possible scenarios that set, get, search and remove specific keys, and iterate all keys.

The example contains an [mbed-lib.json](./mbed-lib.json) configuration file that demonstrates how to set KVStore configuration for a specific target (K64F) that may override the [default configuration](#kvstore-configuration).

## Hardware requirements

KVStore general API may use several types of key/value storage classes, according to the configuration,  that run on top of an underlying Block Device. Each type has different HW requirements for the underlying Block Device.
1. TDB_INTERNAL - may run on top of flash Block Device only (typically internal flash)
2. TDB_EXTERNAL - requires external flash for the key/value storage and typically internal flash for its replay protection storage.
3. TDB_EXTERNAL_NO_RBP - requires external flash for the key/value storage
4. FILESYSTEM - may use either flash or SD card for the key/value storage and typically internal flash for its replay protection storage.
5. FILESYSTEM_NO_RBP - may use either flash or SD card for the key/value storage

## Usage

#### Import the example

Make sure you have an Mbed development environment set up. [Get started with Mbed OS](https://os.mbed.com/docs/latest/tutorials/your-first-program.html)
to set everything up.

From the command-line, import the example:

```
mbed import mbed-os-example-kvstore
cd mbed-os-example-kvstore
```

#### Compile the example

Invoke `mbed compile`, and specify the name of your platform and your favorite
toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m K64F -t ARM
```

Your PC may take a few moments to compile your code. At the end, you'll see a
result similar to:

```
[snip]
+--------------------------+-------+-------+-------+
| Module                   | .text | .data |  .bss |
+--------------------------+-------+-------+-------+
| [fill]                   |   235 |     4 |   345 |
| [lib]/c.a                | 31247 |  2472 |    89 |
| [lib]/gcc.a              |  3112 |     0 |     0 |
| [lib]/misc               |   204 |     4 |    28 |
| [lib]/nosys.a            |    32 |     0 |     0 |
| main.o                   |  1719 |     0 |     0 |
| mbed-os/cmsis            |  1029 |     0 |    84 |
| mbed-os/components       |  4161 |     0 |    48 |
| mbed-os/drivers          |  1487 |     0 |   100 |
| mbed-os/features         | 42061 |     4 |  1426 |
| mbed-os/hal              |  2043 |     4 |    68 |
| mbed-os/platform         |  3095 |   260 |   134 |
| mbed-os/rtos             |  7804 |   168 |  5969 |
| mbed-os/targets          | 15561 |    12 |   413 |
| Subtotals                |113790 |  2928 |  8704 |
+--------------------------+-------+-------+-------+
Total Static RAM memory (data + bss): 11632(+0) bytes
Total Flash memory (text + data): 116718(+0) bytes

Image: ./BUILD/K82F/GCC_ARM/kvstore.bin
```

#### Run the example

1. Connect your Mbed Enabled device to the computer over USB.
1. Copy the binary file to the Mbed Enabled device.
1. Press the reset button to start the program.
1. Open the UART of the board in your favorite UART viewing program. For
   example, `screen /dev/ttyACM0`.

**Note:** The default serial port baud rate is 115200 bit/s.
          You may open serial term with:  
```
mbed sterm -b 115200 -r
```

Expected output:

```
--- Mbed OS KVStore static API example ---
kv_reset
kv_reset -> 0
kv_set first dummy key
kv_set -> 0
kv_get_info of first key
kv_get_info -> 0
kv_get_info key: /kv/dummy_key1
kv_get_info info - size: 31, flags: 0
kv_get first key
kv_get -> 0
kv_get key: /kv/dummy_key1
kv_get value: kvstore_dummy_value_hello_world
kv_set second dummy key
kv_set -> 0
kv_set third key with Confidentiality, Integrity and Replay Protection flags
kv_set -> 0
kv_set Set 'Real' Key 1
kv_set -> 0
kv_set Set 'Real' Key 2 with flag write-once
kv_set -> 0
Removing 'Dummy' Keys
1) Removing dummy_key1
2) Removing dummy_key2
3) Removing dummy_auth_enc_key
Remaining with 'Real' Keys:
1) real_wo_key
2) real_key1
kv_remove write-once file - should fail!
kv_remove -> 274
kv_reset format kvstore (including write-once)
kv_reset -> 0
```

#### Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html)
for suggestions on what could be wrong and how to fix it.

## Default Global KVStore Configuration
The Global KVStore APIs rely on the higher level Mbes-OS KVSTore retargetting layer [json configuration]()
