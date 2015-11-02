#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "skiplist.h"

#define N 100
//#define SKIPLIST_DEBUG

int
main(void)
{
	int i;
	struct timeval start, end;
	struct skiplist *list;
	struct skipnode *node;
	int res = 0;

	int *key = malloc(N * sizeof(int));
	if (key == NULL) {
		printf("-ENOMEM\n");
		return -1;
	}

	printf("Starting initialization...\n");
	list = skiplist_new();
        if (list == NULL) {
		printf("-ENOMEM\n");
		return -1;
        }

	printf("Started initialization...\n");
        printf("Test start!\n");

	/* Test 01 */ 
	printf("Test 01: adding and search %d nodes testing!\n", N);
        printf("Add %d nodes...\n", N);

        gettimeofday(&start, NULL);
        for (i = 0; i < N; i++) {
                int value = key[i] = i;
               	skiplist_insert(list, key[i], value);
        }
        gettimeofday(&end, NULL);
        printf("time span:% ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000);
#ifdef SKIPLIST_DEBUG
        skiplist_dump(list);
#endif

        /* Search test */
        printf("Now search %d node...\n", N);
        gettimeofday(&start, NULL);

        for (i = 0; i < N; i++) {
                struct skipnode *node = skiplist_search(list, key[i]);

                if (node != NULL) {
#ifdef SKIPLIST_DEBUG
                        printf("key:%d value:%d\n", node->key, node->value);
#endif
                } else {
                        printf("Not found:%d\n", key[i]);
			res = 1;
			break;
                }
        }
        gettimeofday(&end, NULL);
        printf("time span:% ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000);
	if (res) {
		printf("Test 01: failed!\n");
		goto out_clean;
	} else {
		printf("Test 01: success!\n");
	}

	/* Test 02 */

	printf("Test 02: search single node (%d/2) testing!\n", N);
	node = skiplist_search(list, N/2);
	if (node && node->value == N/2) {
		printf("Test 02: Success!\n");
	} else {
		printf("Test 02: Failed!\n");
		res = 1;
		goto out_clean;
	}

	/* Test 03 */

	printf("Test 03: remove single node (%d/2) testing!\n", N);
	skiplist_remove(list, N/2);
	node = skiplist_search(list, N/2);
	if (!node) {
		printf("Test 03: Success!\n");
	} else {
		printf("Test 03: Failed (key:%d)!\n", node->key);
		res = 1;
		goto out_clean;
	}

	/* Test 04 */

	printf("Test 04: search single node equal or great than (%d/2) testing!\n", N);
	printf("Test 04: case 1: no equal node (%d/2) \n", N);
	node = skiplist_search_first_eq_big(list, N/2);
	if (!node || node->value != (N/2 + 1)) {
		printf("Test 04: Failed!\n");
		res = 1;
		goto out_clean;
	}

	printf("Test 04: case 2: has equal node (%d/2 + 1) \n", N);
	node = skiplist_search_first_eq_big(list, N/2 + 1);
	if (node && node->value == (N/2 + 1)) {
		printf("Test 04: Success!\n");
	} else {
		printf("Test 04: Failed!\n");
		res = 1;
		goto out_clean;
	}

	/* Test 05 */
	res = 0;
	printf("Test 05: remove all nodes\n");
        for (i = 0; i < N; i++) {
                skiplist_remove(list, key[i]);
        }

        for (i = 0; i < N; i++) {
		node = skiplist_search(list, key[i]);
		if (node) {
			res = 1;
			break;
		}
	}

	if (res)
		printf("Test 05: Failed!\n");
	else
		printf("Test 05: Success!\n");

#ifdef SKIPLIST_DEBUG
        skiplist_dump(list);
#endif

        printf("End of Test.\n");
out_clean:	

        skiplist_delete(list);

        return 0;
}
