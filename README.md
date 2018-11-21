# Getting started with the Mbed OS Key Value Store API

This example demonstrates how to use the Mbed OS Key Value Storage (KVStore) API.
KVStore is a key-value storage based API over a block device.
Mbed-OS provides several KVStore implementation options, that can be optimized to
the specific application requirements and target hardware:
- TDBStore - Default implementation of the KVStore API. It provides static wear-leveling and quick access for when you have a small number of KV pairs.
- FileSystemStore - Class that provides a key-value store API on top of a POSIX-like file system API.
- SecureStore - Class that provides encryption, authentication and rollback protection on top of the KVStore API. It requires two KVStore implementations, one that provides the storage for the KV pairs and one that provides storage for the CMACs that protect KV pairs stored in the KVStore.

You can find more information about Mbed OS Key Value Store Design and other related items in the [kvstore design documentation](https://github.com/ARMmbed/mbed-os/tree/master/docs/design-documents/features/storage/KVStore/KVStore_design.md).

**Table of contents:**

1. [Working with KVStore Class-vs-Global APIs](#working-wit-kvstor)
2. [Examples Class/Global APIs](#examples-global/class-apis)
2. [Hardware requirements](#hardware-requirements)

## Working with KVStore

There are two main options for building a KVStore solution and working with it:
1. Option A: Use Mbed-OS [KVStore configuration](#kvstore-configuration) json file to define your selected KVStore implementation.
2. Option B: Build your own KVStore solution by directly defining the class modules that best fit your applicatio0n and hardware (either TDBStore, FileSystemStore or SecureStore)

Each of the above options has its own set of API functions:
1. Option A: Global APIs - using the higher level static retargetting layer APIs, which rely on [KVStore configuration](#kvstore-configuration)
2. Option B: Class APIs - directly calling the APIs of KVStore class on the implementing class instance (either TDBStore, FileSystemStore or SecureStore)

## Examples Global/Class APIs

Each of the Options above has its own example:
1. Example A: [Global APIs Example](./kvstore_global/README_KVStore_global.md)
2. Example B: [Class APIs Example](./kvstore_api/README_KVStore_api.md)

## Hardware requirements

This example uses several types of key/value storage classes that run on top of an underlying Block Device. Each type has different HW requirements for the underlying Block Device.
1. TDBStore - may run on top of Flash Block Device only
2. FileSystemStore - may run on top of either Flash or SD card Block Device
3. SecureStore - may run on either TDBStore or FileSystemStore for underlying storage and requires TDBStore for its rollback protection storage.
