#pragma once

#include <bits/stdc++.h>

using namespace std;

// Process the longest subsequence s0, ..., s(k-1) satisfying cmp(s0, s1) && ... && cmp(s(k-2), s(k-1))
template<class T, class U = int, class Compare = less<>>
struct longest_monotone_subsequence_processor {
	Compare cmp;
	// less: longest increasing subsequence
	// less_equal: longest non-decreasing subsequence
	// greater: longest decreasing subsequence
	// greater_equal: longest non-increasing subsequence
	int n = 0;
	vector<T> sequence;
	vector<int> active; // position of the optimal last element among monotone sequences of length i+1
	vector<int> prev; // second-to-last element of the longest monotone sequence ending at i, -1 if non exists
	vector<int> length; // length of the longest monotone subsequence ending at i
	vector<U> count; // number of the longest monotone subsequences ending at i
	vector<vector<int>> classes = vector<vector<int>>(1); // classes[len]: set of indices i where length[i] = len (in increasing order)
	vector<int> aux_it = vector<int>(1); // auxiliary position array for counting
	vector<U> aux_sum = vector<U>{ 1 }; // auxiliary sum array for counting
	longest_monotone_subsequence_processor(Compare cmp = less<>()) {}
	longest_monotone_subsequence_processor(const vector<T> &a, Compare cmp = less<>()) {
		insert(a);
	}
	void insert(const vector<T> &a) {
		for (auto x : a) insert(x);
	}
	void insert(T x) { // O(log n)
		int i = n ++;
		sequence.push_back(x);
		prev.push_back(-1);
		length.push_back(-1);
		count.push_back(0);
		auto it = partition_point(active.begin(), active.end(), [&](int i) { return cmp(sequence[i], x); });
		if (it == active.end()) {
			active.emplace_back();
			classes.emplace_back();
			aux_it.emplace_back();
			aux_sum.emplace_back();
			it = std::prev(active.end());
		}
		*it = i;
		prev[i] = it == active.begin() ? -1 : *std::prev(it);
		int len = length[i] = it - active.begin() + 1;
		while (aux_it[len - 1] < (int)classes[len - 1].size() && !cmp(sequence[classes[len - 1][aux_it[len - 1]]], x)) {
			aux_sum[len - 1] -= count[classes[len - 1][aux_it[len - 1] ++]];
		}
		count[i] = aux_sum[len - 1];
		classes[len].push_back(i);
		aux_sum[len] += count[i];
	}
	int lms_length() const {
		return (int)active.size();
	}
	vector<int> lms_indices() const {
		int len = lms_length(), i = active.back();
		vector<int> res(len);
		while (len --) res[len] = i, i = prev[i];
		return res;
	}
	vector<T> lms() const {
		vector<T> res;
		res.reserve(lms_length());
		for (auto i : lms_indices()) res.push_back(sequence[i]);
		return res;
	}
	U lms_count() const {
		U res = 0;
		int len = lms_length();
		for (auto i = 0; i < n; ++ i) if (length[i] == len) res += count[i];
		return res;
	}
};