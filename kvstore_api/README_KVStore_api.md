# Mbed OS Key Value Store (KVStore) Class API

This example demonstrates how to use the Mbed OS Key Value Storage ([KVStore](../README.md)) Class API.

The example sets several key/value pairs, some are defined as 'dummy' and some as 'real'. The example demonstrates several possible scenarios that set, get, search and remove specific keys, and iterate all keys.

A similar scenario is run on each of the possible KVStore class modules:

- TDBStore - providing static wear-leveling and quick access, when you have a small number of KV pairs.
- FileSystemStore - providing a key-value store API on top of a POSIX-like file system API.
- SecureStore - provideing encryption, authentication and rollback protection on top of the KVStore API (requires two KVStore instances: one that provides the storage for the KV pairs and one that provides playback attack protection)



**Table of contents:**

1. [Usage](#usage)
   - [Import the example](#import-the-example)
   - [Compile the example](#compile-the-example)
   - [Run the example](#run-the-example)
   - [Troubleshooting](#troubleshooting)

## Usage

#### Import the example

Make sure you have an Mbed development environment set up. [Get started with Mbed OS](https://os.mbed.com/docs/latest/tutorials/your-first-program.html)
to set everything up.

From the command-line, import the example:

```
mbed import mbed-os-example-kvstore
cd mbed-os-example-kvstore/kvstore_api
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

Image: ./BUILD/K82F/GCC_ARM/kvstore_api.bin
```

#### Run the example

1. Connect your Mbed Enabled device to the computer over USB.
1. Copy the binary file to the Mbed Enabled device.
1. Press the reset button to start the program.
1. Open the UART of the board in your favorite UART viewing program. For
   example, `screen /dev/ttyACM0`.

**Note:** The default serial port baud rate is 9600 bit/s.

Expected output:

```
--- Mbed OS TDBStore API example ---
tdb_store->init
tdb_store->init -> 0
tdb_reset
tdb_reset -> 0
tdb_set first dummy key
tdb_set -> 0
tdb_get_info of first key
tdb_get_info -> 0
tdb_get_info key: dummy_key1
tdb_get_info info - size: 31, flags: 0
tdb_get first key
tdb_get -> 0
tdb_get key: dummy_key1
tdb_get value: kvstore_dummy_value_hello_world
tdb_set second dummy key
tdb_set -> 0
tdb_set third key with Confidentiality, Integrity and Replay Protection flags
(successful on SecureStore only)
tdb_set -> 0
tdb_set Set 'Real' Key 1
tdb_set -> 0
tdb_set Set 'Real' Key 2 with flag write-once
tdb_set -> 0
Removing 'Dummy' Keys
1) Removing dummy_key1
2) Removing dummy_key2
3) Removing dummy_auth_enc_key
Remaining with 'Real' Keys:
1) real_wo_key
2) real_key1
tdb_remove write-once file - should fail!
tdb_remove -> 274
tdb_reset format kvstore (including write-once)
tdb_reset -> 0
tdb_store->deinit
tdb_store->deinit -> 0
--- Mbed OS FileSystemStore API example ---
file_system_store->init
file_system_store->init -> 0
file_system_reset
file_system_reset -> 0
file_system_set first dummy key
file_system_set -> 0
file_system_get_info of first key
file_system_get_info -> 0
file_system_get_info key: dummy_key1
file_system_get_info info - size: 31, flags: 0
file_system_get first key
file_system_get -> 0
file_system_get key: dummy_key1
file_system_get value: kvstore_dummy_value_hello_world
file_system_set second dummy key
file_system_set -> 0
file_system_set third key with Confidentiality, Integrity and Replay Protection
     flags (successful on SecureStore only)
file_system_set -> 0
file_system_set Set 'Real' Key 1
file_system_set -> 0
file_system_set Set 'Real' Key 2 with flag write-once
file_system_set -> 0
Removing 'Dummy' Keys
1) Removing dummy_key1
2) Removing dummy_key2
3) Removing dummy_auth_enc_key
Remaining with 'Real' Keys:
1) real_key1
2) real_wo_key
file_system_remove write-once file - should fail!
file_system_remove -> 274
file_system_reset format kvstore (including write-once)
file_system_reset -> 0
file_system_store->deinit
file_system_store->deinit -> 0
--- Mbed OS SecureStore API example ---
secure_store->init
secure_store->init -> 0
secure_reset
secure_reset -> 0
secure_set first dummy key
secure_set -> 0
secure_get_info of first key
secure_get_info -> 0
secure_get_info key: dummy_key1
secure_get_info info - size: 31, flags: 0
secure_get first key
secure_get -> 0
secure_get key: dummy_key1
secure_get value: kvstore_dummy_value_hello_world
secure_set second dummy key
secure_set -> 0
secure_set third key with Confidentiality, Integrity and Replay Protection
   flags (successful on SecureStore only)
secure_set -> 0
secure_set Set 'Real' Key 1
secure_set -> 0
secure_set Set 'Real' Key 2 with flag write-once
secure_set -> 0
Removing 'Dummy' Keys
1) Removing dummy_key1
2) Removing dummy_key2
3) Removing dummy_auth_enc_key
Remaining with 'Real' Keys:
1) real_wo_key
2) real_key1
secure_remove write-once file - should fail!
secure_remove -> 274
secure_reset format kvstore (including write-once)
secure_reset -> 0
secure_store->deinit
secure_store->deinit -> 0
```

#### Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html)
for suggestions on what could be wrong and how to fix it.
