#pragma once
#include "../lib/num.h"
#include "../lib/string.h"
#include "../mem/heap.h"

template <typename T>
class List {
    public:
        T* items;
        u64 length;
        u64 capacity;

        List() {
            items = (T*)malloc(sizeof(T) * 4);
            length = 0;
            capacity = 4;
        }

        List(u64 capacity) {
            items = (T*)malloc(sizeof(T) * capacity);
            length = 0;
            this->capacity = capacity;
        }

        void add(T item) {
            if (length == capacity) {
                capacity *= 2;
                T* new_items = (T*)malloc(sizeof(T) * capacity);
                for (u64 i = 0; i < length; i++) {
                    new_items[i] = items[i];
                }
                free(items);
                items = new_items;
            }

            items[length] = item;
            length++;
        }

        void remove(u64 index) {
            if (index >= length) {
                return;
            }

            for (u64 i = index; i < length - 1; i++) {
                items[i] = items[i + 1];
            }

            length--;
        }

        void remove(T item) {
            for (u64 i = 0; i < length; i++) {
                if (items[i] == item) {
                    remove(i);
                    return;
                }
            }
        }

        void clear() {
            length = 0;
        }

        T get(u64 index) {
            if (index >= length) {
                return NULL;
            }

            return items[index];
        }

        void set(u64 index, T item) {
            if (index >= length) {
                return;
            }

            items[index] = item;
        }

        bool contains(T item) {
            for (u64 i = 0; i < length; i++) {
                if (items[i] == item) {
                    return true;
                }
            }

            return false;
        }

        u64 indexOf(T item) {
            for (u64 i = 0; i < length; i++) {
                if (items[i] == item) {
                    return i;
                }
            }

            return -1;
        }

        void sort() {
            for (u64 i = 0; i < length; i++) {
                for (u64 j = 0; j < length - i - 1; j++) {
                    if (items[j] > items[j + 1]) {
                        T temp = items[j];
                        items[j] = items[j + 1];
                        items[j + 1] = temp;
                    }
                }
            }
        }

        void reverse() {
            for (u64 i = 0; i < length / 2; i++) {
                T temp = items[i];
                items[i] = items[length - i - 1];
                items[length - i - 1] = temp;
            }
        }
};