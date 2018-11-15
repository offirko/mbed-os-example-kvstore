/* Copyright (c) 2018 Arm Limited
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "mbed.h"
#include <stdio.h>
#include "KVStore.h"
#include "kvstore_global_api.h"
#include "BlockDevice.h"
#include "FileSystem.h"
#include "FileSystemStore.h"

using namespace mbed;

#define EXAMPLE_KV_VALUE_LENGTH 64
#define EXAMPLE_KV_KEY_LENGTH 16

void kv_store_global_api_example();
void file_system_store_api_example();

int main()
{
    kv_store_global_api_example();
    file_system_store_api_example();
}

void file_system_store_api_example()
{
    char kv_value_in[EXAMPLE_KV_VALUE_LENGTH] = {"kvstore_example_value_hello_world"};
    char kv_key_in[EXAMPLE_KV_KEY_LENGTH] = {"example_key"};
    char kv_value_out[EXAMPLE_KV_VALUE_LENGTH] = {0};
    char kv_key_out[EXAMPLE_KV_KEY_LENGTH] = {0};
    size_t actual_size = 0;
    KVStore::info_t info;
    KVStore::iterator_t kvstore_it;
    int i_ind = 0;

    printf("--- Mbed OS FileSystemStore API example ---\n");

    int res = MBED_ERROR_NOT_READY;

    BlockDevice *bd = BlockDevice::get_default_instance();

    res = bd->init();

    FileSystem *fs = FileSystem::get_default_instance();

    res = fs->mount(bd);
    if (res) {
        res = fs->reformat(bd);
    }

    FileSystemStore *fsst = new FileSystemStore(fs);

    printf("fsst->init\n");
    res = fsst->init();
    printf("fsst->init -> %d\n", res);

    printf("fsst->reset\n");
    res = fsst->reset();
    printf("fsst->reset -> %d\n", res);

    printf("fsst->get from empty kvstore - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    res = fsst->get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("fsst->get -> %d\n", res);

    printf("fsst->set first key\n");
    res = fsst->set(kv_key_in, kv_value_in, strlen(kv_value_in), 0);
    printf("fsst->set -> %d\n", res);

    printf("fsst->set second key with write-once flag\n");
    res = fsst->set("wo_key", "wo_value", strlen("wo_value"), KVStore::WRITE_ONCE_FLAG);
    printf("fsst->set -> %d\n", res);

    printf("fsst->get first key\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("fsst->get -> %d\n", res);
    printf("fsst->get key: %s\n", kv_key_in);
    printf("fsst->get value: %s\n", kv_value_out);

    printf("fsst->get_info of first key\n");
    res = fsst->get_info(kv_key_in, &info);
    printf("fsst->get_info -> %d\n", res);
    printf("fsst->get_info key: %s\n", kv_key_in);
    printf("fsst->get_info info - size: %u, flags: %lu\n", info.size, info.flags);

    printf("Iterating Existing Keys\n");
    res = fsst->iterator_open(&kvstore_it, NULL);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    while (fsst->iterator_next(kvstore_it, kv_key_out, EXAMPLE_KV_KEY_LENGTH) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
        printf("%d) %s\n", i_ind, kv_key_out);
        memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    }
    res = fsst->iterator_close(kvstore_it);

    printf("fsst->remove first key\n");
    res = fsst->remove(kv_key_in);
    printf("fsst->remove -> %d\n", res);

    printf("fsst->get first key after removing it - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = fsst->get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("fsst->get -> %d\n", res);

    printf("fsst->remove write-once file - should fail!\n");
    res = fsst->remove("wo_key");
    printf("fsst->remove -> %d\n", res);

    printf("fsst->get second key after failing to remove it\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = fsst->get("wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("fsst->get -> %d\n", res);
    printf("fsst->get value: %s\n", kv_value_out);

    printf("fsst->reset will format kvstore (including write-once)\n");
    res = fsst->reset();
    printf("fsst->reset -> %d\n", res);

    printf("fsst->get second key after reset - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = fsst->get("wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("fsst->get -> %d\n", res);

    printf("fsst->deinit\n");
    res = fsst->deinit();
    printf("fsst->deinit -> %d\n", res);

    bd->deinit();

    return;
}

void kv_store_global_api_example()
{
    char kv_value_in[EXAMPLE_KV_VALUE_LENGTH] = {"kvstore_example_value_hello_world"};
    char kv_key_in[EXAMPLE_KV_KEY_LENGTH] = {"/kv/example_key"};
    char kv_value_out[EXAMPLE_KV_VALUE_LENGTH] = {0};
    char kv_key_out[EXAMPLE_KV_KEY_LENGTH] = {0};
    size_t actual_size = 0;
    kv_info_t info;
    kv_iterator_t kvstore_it;
    int i_ind = 0;

    printf("--- Mbed OS KVStore static API example ---\n");

    int res = MBED_ERROR_NOT_READY;

    printf("kv_reset\n");
    res = kv_reset("/kv/");
    printf("kv_reset -> %d\n", res);

    printf("kv_get from empty kvstore - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", res);

    printf("kv_set first key\n");
    res = kv_set(kv_key_in, kv_value_in, strlen(kv_value_in), 0);
    printf("kv_set -> %d\n", res);

    printf("kv_set second key with write-once flag\n");
    res = kv_set("/kv/wo_key", "wo_value", strlen("wo_value"), KVStore::WRITE_ONCE_FLAG);
    printf("kv_set -> %d\n", res);

    printf("kv_get first key\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", res);
    printf("kv_get key: %s\n", kv_key_in);
    printf("kv_get value: %s\n", kv_value_out);

    printf("kv_get_info of first key\n");
    res = kv_get_info(kv_key_in, &info);
    printf("kv_get_info -> %d\n", res);
    printf("kv_get_info key: %s\n", kv_key_in);
    printf("kv_get_info info - size: %u, flags: %lu\n", info.size, info.flags);

    printf("Iterating Existing Keys\n");
    res = kv_iterator_open(&kvstore_it, NULL);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    while (kv_iterator_next(kvstore_it, kv_key_out, EXAMPLE_KV_KEY_LENGTH) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
        printf("%d) %s\n", i_ind, kv_key_out);
        memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    }
    res = kv_iterator_close(kvstore_it);

    printf("kv_remove first key\n");
    res = kv_remove(kv_key_in);
    printf("kv_remove -> %d\n", res);

    printf("kv_get first key after removing it - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", res);

    printf("kv_remove write-once file - should fail!\n");
    res = kv_remove("/kv/wo_key");
    printf("kv_remove -> %d\n", res);

    printf("kv_get second key after failing to remove it\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get("/kv/wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", res);
    printf("kv_get value: %s\n", kv_value_out);

    printf("kv_reset will format kvstore (including write-once)\n");
    res = kv_reset("/kv/");
    printf("kv_reset -> %d\n", res);

    printf("kv_get second key after reset - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get("/kv/wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", res);

    return;
}
