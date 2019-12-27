#include <bits/stdc++.h>

using namespace std;

class Bucket {
    int capacity;
    int local_depth;

    vector < pair <unsigned int, unsigned int> > datas;

    public:
        Bucket(int local_depth, int capacity) {
            this->capacity = capacity;
            this->local_depth = local_depth;
        };

        int insert(unsigned int key, unsigned int value) {
            assert(value > 0);
            if (datas.size() == capacity) {
                return 0; //insert failed...
            }
            datas.push_back(make_pair(key, value));
            return 1;
        }

        int search(unsigned int key) {
            for (auto x: this->datas) {
                if (x.first == key)
                    return x.second;
            }
            return 0; //not found
        }

        int depth() {
            return local_depth;
        }

        pair <Bucket*, Bucket*> split() {
            int N = 1 << local_depth;
            Bucket *new_bucket = new Bucket(local_depth + 1, capacity);
            Bucket *original_bucket = new Bucket(local_depth + 1, capacity);
            for (auto x: this->datas) {
                if (x.first & N) {
                    assert(new_bucket->insert(x.first, x.second) == 1);
                } else {
                    assert(original_bucket->insert(x.first, x.second) == 1);
                }
            }
            return make_pair(original_bucket, new_bucket);
        }

        void display(bool with_value = false) {
            for (auto x: this->datas) {
                cout << "key: " << x.first;
                if (with_value) {
                    cout << " value: " << x.second;
                }
                cout << " ;";
            }
        }

};

class Directory {
    int global_depth;
    int bucket_capacity;

    vector <Bucket *> buckets;

        unsigned int which_bucket(unsigned int key);

    public:
        unsigned int bucket_num() {
            return 1 << global_depth;
        }

        Directory(int global_depth, int bucket_capacity) {
            this->global_depth = global_depth;
            this->bucket_capacity = bucket_capacity;
            for (int i = 0; i < bucket_num(); i++) {
                buckets.push_back(new Bucket(global_depth, bucket_capacity));
            }
        }

        void expand() {
            for (int i = 0; i < bucket_num(); i++)
                buckets.push_back(buckets[i]);
            this->global_depth++;
        }

        void insert(unsigned int key, unsigned int value) {
            while (true) {
                auto idx = which_bucket(key);
                auto success = buckets[idx]->insert(key, value);
                if (success) break;
                auto local_depth = buckets[idx]->depth();
                if (local_depth == global_depth)
                    this->expand();
                auto ptrs = buckets[idx]->split();

                auto step = 1 << buckets[idx]->depth();

                auto pre_ptr = buckets[idx];

                for (auto i = idx & (step - 1); i < bucket_num() ; i+= step << 1) {
                    buckets[i] = ptrs.first;
                    auto j = i + step;
                    if (j >= bucket_num()) continue;
                    buckets[j] = ptrs.second;
                }
                delete pre_ptr;
            }
        }

        unsigned int search(unsigned int key) {
            int idx = this->which_bucket(key);
            return buckets[idx]->search(key);
        }

        void display() {
            cout << "start to display directory... "<< "global_depth = " << global_depth << endl;

            for (int i = 0; i < buckets.size(); i++) {
                cout << "bucket  " << i << " " << "local_depth = " << buckets[i]->depth() << " :";
                buckets[i]->display();
                cout << endl;
            }

            cout << "directory display finished." << endl << endl;
        }

};

unsigned int Directory::which_bucket(unsigned int key) {
    return key & (Directory::bucket_num() - 1);
}

// example of https://youtu.be/TtkN2xRAgv4
void test() {
    cout << "start test  ..... " << endl;
    auto d = new Directory(2, 3);
    vector <int> keys = {4, 24, 16, 6, 22, 10, 7, 31};

    for (auto k : keys)
        d->insert(k, 1);

    d->display();

    d->insert(9, 1);
    d->display();

    d->insert(20, 1);
    d->display();

    d->insert(26, 1);
    d->display();

}

int main() {
    test();
    return 0;
}
