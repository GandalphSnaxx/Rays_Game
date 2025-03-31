/**
 * @file deleteQueue.hpp
 * @author Ray Richter
 * @brief Deletion queue. Add delete functions to a queue then flush the queue to call the deletion functions in reverse add order.
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef DELETE_QUEUE_HPP
#define DELETE_QUEUE_HPP

#include <deque>
#include <functional>
#include <vector>

// Storage for deletion functions that get called in lifo order.
struct DeletionQueue {
	std::deque<std::function<void()>> deletors;

	void add(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// Reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); // Call functors
		}
		deletors.clear();
	}
};

#endif // DELETE_QUEUE_HPP