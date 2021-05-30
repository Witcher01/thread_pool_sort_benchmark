#include <unistd.h>

#include "thread_pool.hpp"

#define NR_OF_VECTORS 1000
#define NR_OF_VALUES 100000

void thread_pool_benchmark(size_t nr_of_threads);
void sort_things(std::vector<int> *v);

int main(int argc, char *argv[])
{
	std::cout << "Benchmarking with " << NR_OF_VECTORS << " vectors, each containing " << NR_OF_VALUES << " values." << std::endl;
	std::cout << "CPU supports " << std::thread::hardware_concurrency() << " concurrent number of threads." << std::endl;

	for (int i = 0; i < 10; i++) {
		std::cout << "Benchmarking with " << (1 << i) << " thread(s)" << std::endl;
		thread_pool_benchmark(1 << i);
		std::cout << std::endl;
	}

	std::cout << "Benchmarking done." << std::endl;

	return 0;
}

void thread_pool_benchmark(size_t nr_of_threads)
{
	thread_pool pool(nr_of_threads);

	// timer to measure initialization of vectors
	timer rand;
	// timer to measure sorting via the thread pool
	timer tp;

	rand.start();
	// NR_OF_VECTORS vectors with NR_OF_VALUES ints per vector
	std::vector<int> *vecs[NR_OF_VECTORS];

	std::srand(time(NULL));

	// initialize vectors for sorting
	for (int i = 0; i < NR_OF_VECTORS; i++) {
		std::vector<int> *vec = new std::vector<int>(NR_OF_VALUES);
		for (int j = 0; j < NR_OF_VALUES; j++) {
			vec->push_back(std::rand() % 100);
		}
		vecs[i] = vec;
	}
	rand.stop();
	std::cout << "Initialized vectors in " << rand.ms() << "ms" << std::endl;

	// start measuring thread pool sorting
	tp.start();
	// start sorting tasks via the thread_pool
	for (int i = 0; i < NR_OF_VECTORS; i++) {
		pool.push_task(sort_things, vecs[i]);
	}

	// wait for all tasks to finish
	pool.wait_for_tasks();
	// stop measuring sorting time
	tp.stop();
	std::cout << "Finished sorting arrays in " << tp.ms() << "ms" << std::endl;

	// clean up heap
	std::cout << "Cleaning up..." << std::endl;

	// destroy heap allocated vectors
	for (int i = 0; i < NR_OF_VECTORS; i++) {
		delete vecs[i];
	}
}

void sort_things(std::vector<int> *v)
{
	std::sort(v->begin(), v->end());
}
