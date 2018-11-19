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
#include "kvstore_global_api.h"

using namespace mbed;

#define EXAMPLE_KV_VALUE_LENGTH 64
#define EXAMPLE_KV_KEY_LENGTH 16
#define err_code(res) MBED_GET_ERROR_CODE(res)

void kv_store_global_api_example();

int main()
{
    BlockDevice *bd = BlockDevice::get_default_instance();
    int res = bd->init();

    /* KV Store Static API Example */
    kv_store_global_api_example();

    bd->deinit();
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
    printf("kv_reset -> %d\n", err_code(res));

    printf("kv_get from empty kvstore - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    memset(kv_key_out, 0, EXAMPLE_KV_KEY_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", err_code(res));

    printf("kv_set first key\n");
    res = kv_set(kv_key_in, kv_value_in, strlen(kv_value_in), 0);
    printf("kv_set -> %d\n", err_code(res));

    printf("kv_set second key with write-once flag\n");
    res = kv_set("/kv/wo_key", "wo_value", strlen("wo_value"), KV_WRITE_ONCE_FLAG);
    printf("kv_set -> %d\n", err_code(res));

    printf("kv_set third key with Confidentiality, Integrity and Replay Protection flags seg\n");
    res = kv_set("/kv/auth_enc_key", "auth_enc_value", strlen("auth_enc_value"),
    		KV_REQUIRE_CONFIDENTIALITY_FLAG|KV_REQUIRE_INTEGRITY_FLAG|KV_REQUIRE_REPLAY_PROTECTION_FLAG);
    printf("kv_set -> %d\n", err_code(res));

    printf("kv_get first key\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", err_code(res));
    printf("kv_get key: %s\n", kv_key_in);
    printf("kv_get value: %s\n", kv_value_out);

    printf("kv_get_info of first key\n");
    res = kv_get_info(kv_key_in, &info);
    printf("kv_get_info -> %d\n", err_code(res));
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
    printf("kv_remove -> %d\n", err_code(res));

    printf("kv_get first key after removing it - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get(kv_key_in, kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", err_code(res));

    printf("kv_remove write-once file - should fail!\n");
    res = kv_remove("/kv/wo_key");
    printf("kv_remove -> %d\n", err_code(res));

    printf("kv_get second key after failing to remove it\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get("/kv/wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", err_code(res));
    printf("kv_get value: %s\n", kv_value_out);

    printf("kv_reset will format kvstore (including write-once)\n");
    res = kv_reset("/kv/");
    printf("kv_reset -> %d\n", err_code(res));

    printf("kv_get second key after reset - should fail!\n");
    memset(kv_value_out, 0, EXAMPLE_KV_VALUE_LENGTH);
    res = kv_get("/kv/wo_key", kv_value_out, EXAMPLE_KV_VALUE_LENGTH, &actual_size);
    printf("kv_get -> %d\n", err_code(res));

    return;
}
