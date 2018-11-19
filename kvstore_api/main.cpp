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
#include <string.h>
#include "KVStore.h"
#include "BlockDevice.h"
#include "HeapBlockDevice.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "FileSystem.h"
#include "kvstore_global_api.h"
#include "TDBStore.h"
#include "FileSystemStore.h"
#include "SecureStore.h"

using namespace mbed;

#define EXAMPLE_KV_VALUE_LENGTH 64
#define EXAMPLE_KV_KEY_LENGTH 16
#define err_code(res) MBED_GET_ERROR_CODE(res)

void kv_store_api_example(KVStore *kv_store, const char *kvstore_str);

int main()
{
    BlockDevice *bd = BlockDevice::get_default_instance();
    int res = bd->init();
    const size_t ul_bd_size  = 8 * 4096;
    const size_t rbp_bd_size = 4 * 4096;

    /* TDBStore can not work on SD Card BD */
#if COMPONENT_SD
    HeapBlockDevice heap_bd(ul_bd_size + rbp_bd_size, 1, 1, 4096);
    FlashSimBlockDevice *flash_bd = new FlashSimBlockDevice(&heap_bd);
#else
    BlockDevice *flash_bd = bd;
#endif

    /* TDBStore Example */
    printf("--- Mbed OS TDBStore API example ---\n");
    TDBStore *tdb = new TDBStore(flash_bd);
    kv_store_api_example(tdb, "tdb");
    delete tdb;

    /* FileSystemStore Example */
    printf("--- Mbed OS FileSystemStore API example ---\n");
    FileSystem *fs = FileSystem::get_default_instance();
    res = fs->mount(bd);
    if (res) {
        res = fs->reformat(bd);
    }
    FileSystemStore *fsst = new FileSystemStore(fs);
    kv_store_api_example(fsst, "file_system");
    delete fsst;

    /* SecureStore Example */
    printf("--- Mbed OS SecureStore API example ---\n");
    SlicingBlockDevice ul_bd(flash_bd, 0, ul_bd_size);
    SlicingBlockDevice rbp_bd(flash_bd, ul_bd_size, ul_bd_size + rbp_bd_size);
    TDBStore *tdb_ul = new TDBStore(&ul_bd);
    TDBStore *tdb_rbp = new TDBStore(&rbp_bd);
    SecureStore *scst = new SecureStore(tdb_ul, tdb_rbp);
    kv_store_api_example(scst, "secure");
    delete scst;
    delete tdb_ul;
    delete tdb_rbp;

    bd->deinit();
}

void kv_store_api_example(KVStore *kv_store, const char *kvstore_str)
{
    char kv_value_in[EXAMPLE_KV_VALUE_LENGTH] = {"kv_store_example_value_hello_world"};
    char kv_key_in[EXAMPLE_KV_KEY_LENGTH] = {"example_key"};
    char kv_value_out[EXAMPLE_KV_VALUE_LENGTH] = {0};
    char kv_key_out[EXAMPLE_KV_KEY_LENGTH] = {0};
    size_t actual_size = 0;
    KVStore::info_t info;
    KVStore::iterator_t kvstore_it;
    int i_ind = 0;
    int res = MBED_ERROR_NOT_READY;

    printf("%s_store->init\n", kvstore_str);
    res = kv_store->init();
    printf("%s_store->init -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->reset\n", kvstore_str);
    res = kv_store->reset();
    printf("%s_store->reset -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->get from empty kvstore - should fail!\n", kvstore_str);
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    res = kv_store->get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("%s_store->get -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->set first key\n", kvstore_str);
    res = kv_store->set(kv_key_in, kv_value_in, strlen(kv_value_in), 0);
    printf("%s_store->set -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->set second key with write-once flag\n", kvstore_str);
    res = kv_store->set("wo_key", "wo_value", strlen("wo_value"), KV_WRITE_ONCE_FLAG);
    printf("%s_store->set -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->set third key with Confidentiality, Integrity and Replay Protection flags seg\n", kvstore_str);
    printf("%s_store->set should succeed for SecureStore only\n", kvstore_str);
    res = kv_store->set("auth_enc_key", "auth_enc_value", strlen("auth_enc_value"),	KV_REQUIRE_CONFIDENTIALITY_FLAG|KV_REQUIRE_INTEGRITY_FLAG|KV_REQUIRE_REPLAY_PROTECTION_FLAG);
    printf("%s_store->set -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->get first key\n", kvstore_str);
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_store->get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("%s_store->get -> %d\n", kvstore_str, err_code(res));
    printf("%s_store->get key: %s\n", kvstore_str, kv_key_in);
    printf("%s_store->get value: %s\n", kvstore_str, kv_value_out);

    printf("%s_store->get_info of first key\n", kvstore_str);
    res = kv_store->get_info(kv_key_in, &info);
    printf("%s_store->get_info -> %d\n", kvstore_str, err_code(res));
    printf("%s_store->get_info key: %s\n", kvstore_str, kv_key_in);
    printf("%s_store->get_info info - size: %u, flags: %lu\n", kvstore_str, info.size, info.flags);

    printf("Iterating Existing Keys\n");
    res = kv_store->iterator_open(&kvstore_it, NULL);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    while (kv_store->iterator_next(kvstore_it, kv_key_out, EXAMPLE_KV_KEY_LENGTH) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
        printf("%d) %s\n", i_ind, kv_key_out);
        memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    }
    res = kv_store->iterator_close(kvstore_it);

    printf("%s_store->remove first key\n", kvstore_str);
    res = kv_store->remove(kv_key_in);
    printf("%s_store->remove -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->get first key after removing it - should fail!\n", kvstore_str);
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_store->get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("%s_store->get -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->remove write-once file - should fail!\n", kvstore_str);
    res = kv_store->remove("wo_key");
    printf("%s_store->remove -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->get second key after failing to remove it\n", kvstore_str);
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_store->get("wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("%s_store->get -> %d\n", kvstore_str, err_code(res));
    printf("%s_store->get value: %s\n", kvstore_str, kv_value_out);

    printf("%s_store->reset will format kvstore (including write-once)\n", kvstore_str);
    res = kv_store->reset();
    printf("%s_store->reset -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->get second key after reset - should fail!\n", kvstore_str);
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_store->get("wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("%s_store->get -> %d\n", kvstore_str, err_code(res));

    printf("%s_store->deinit\n", kvstore_str);
    res = kv_store->deinit();
    printf("%s_store->deinit -> %d\n", kvstore_str, err_code(res));

    return;
}
