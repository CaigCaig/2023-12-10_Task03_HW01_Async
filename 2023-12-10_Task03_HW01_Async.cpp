#include <iostream>
#include <Windows.h>
#include <thread>
#include <random>
#include <future>

using namespace std;

constexpr int size_v = 20;

void search_min(vector<int> data, int i, promise<int>prom)
{
	auto val = i;
	for (auto j = i + 1; j < data.size(); j++) if (data[j] < data[val]) val = j;
	prom.set_value(val);
}

vector<int> selection_sort(vector<int> arr, int size)
{
	for (auto i = 0; i < size; ++i)
	{
		promise<int>prom;
		future<int> i_swap = prom.get_future();
		async(search_min, arr, i, move(prom));
		swap(arr[i], arr[i_swap.get()]);
	}
	return arr;
}

int main()
{
	vector<int> data(size_v);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> rnd(0, 100);
	for (auto i = 0; i < size_v; ++i)
	{
		data[i] = rnd(rd);
		cout << data[i] << ' ';
	}
	cout << endl;

	auto sort_data = async(selection_sort, data, size_v);
	auto vec = sort_data.get();

	for (auto i = 0; i < size_v; ++i) cout << vec[i] << ' ';
	cout << endl;

	system("pause");
}

/*
Сортировка выбором
Реализуйте алгоритм сортировки выбором.
Сортировка выбором состоит в том, что в неупорядоченном списке находится наименьший элемент. Выбранный в исходном списке минимальный элемент записывается на i-е место исходного списка (i=1,2,…,п), а элемент с i-го места — на место выбранного.
При этом очевидно, что уже упорядоченные элементы, а они будут расположены, начиная с первого места, исключаются из дальнейшей сортировки, поэтому длина списка, участвующего в каждом последующем просмотре, должна быть на один элемент меньше предыдущего.
Поиск минимального элемента реализуйте в виде асинхронной задачи.
Результат должен возвращаться в основной поток через связку std::promise-std::future.
*/