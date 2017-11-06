/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <unordered_map>
#include <unordered_set>

template <class K, class V>
using umap = std::unordered_map<K, V>;

template <class K>
using uset = std::unordered_set<K>;
