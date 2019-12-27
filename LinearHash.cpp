#include <bits/stdc++.h>

using namespace std;


class Bucket {

    int capacity;

    vector < pair <unsigned int, unsigned int> > datas;

    public:
        Bucket(int capacity) {
            this->capacity = capacity;
        };

        bool is_overflow() {
            return this->datas.size() > this->capacity;
        };

        //assert value > 0
        void insert(unsigned int key, unsigned int value) {
            assert (value > 0);
            this->datas.push_back(make_pair(key, value));
        }

        unsigned int search(unsigned int key) {
            for (auto x: this->datas) {
                if (x.first == key)
                    return x.second;
            }
            return 0; //not found
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

        pair <Bucket*, Bucket*> split(int N) {
            Bucket *new_bucket = new Bucket(this->capacity);
            Bucket *original_bucket = new Bucket(this->capacity);
            for (auto x: this->datas) {
                if (x.first & N) {
                    new_bucket->insert(x.first, x.second);
                } else {
                    original_bucket->insert(x.first, x.second);
                }
            }
            return make_pair(original_bucket, new_bucket);
        }

};

unsigned long hash_dbj2(string str) {
    unsigned long hash = 5381;
    for (auto c : str) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}


class Hashmap {

    int N;
    int d;
    int next;

    vector <Bucket *> buckets;

        int which_bucket(unsigned int key) {
            unsigned int idx = key & (N-1);
            if (idx < next) {
                idx |= key & N;
            }
            return idx;
        }


    public:
        Hashmap(int init_size, int bucket_capacity) {
            assert ((init_size & (init_size - 1)) == 0);
            this->N = init_size;
            this->next = 0;
            for (int i = 0; i < this->N; i++)
                this->buckets.push_back(new Bucket(bucket_capacity));

        }

        void insert(unsigned int key, unsigned int value) {
            //1. insert
            //2. if overflow then split the bucket pointed by next
            //3. if next == N then...
            int idx = this->which_bucket(key);
            buckets[idx]->insert(key, value);

            if (buckets[idx]->is_overflow()) {
                //split buckets[next]
                auto ptrs = buckets[next]->split(N);
                delete buckets[next];
                buckets[next] = ptrs.first;
                buckets.push_back(ptrs.second);
                next+=1;
            }

            if (next == N) {
                next = 0;
                N = N << 1;
            }

        }

        unsigned int search(unsigned int key) {
            int idx = this->which_bucket(key);
            return buckets[idx]->search(key);
        }

        void display() {
            cout << "start to display hashtable ... "<< "N = " << N << ", next = " << next << endl;

            for (int i = 0; i < buckets.size(); i++) {
                cout << "bucket  " << i << ": ";
                buckets[i]->display();
                cout << endl;
            }

            cout << "hashtable display finished." << endl << endl;
        }

};

// example of https://youtu.be/h37Jhr21ByQ
void test() {
    cout << "start test  ..... " << endl;
    auto h = new Hashmap(1 << 2, 4);
    vector <int> keys = {32, 44, 36, 9, 25, 5, 14, 18, 10, 30, 31, 35, 7, 11};

    for (auto k : keys)
        h->insert(k, 1);

    h->display();

    h->insert(37, 1);
    h->display();

    h->insert(43, 1);
    h->display();

    h->insert(29, 1);
    h->display();

    h->insert(22, 1);
    h->display();

    h->insert(66, 1);
    h->insert(34, 1);
    h->insert(50, 1);
    h->display();
}

int main() {
    test();
    return 0;
}
