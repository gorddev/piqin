#pragma once

#define INIT_QUEUE_SIZE 100


namespace gutils {
	// Allows us to keep track of ids easily and re-use them (stack)
	class IDStack {
	private:
		// List of all IDs
		std::vector<int> IDs;
		// Top of the stack
		int front = 0;
		int stackSize = INIT_QUEUE_SIZE;
		bool empty = true;

	public:
		//
		IDStack() {
			IDs.reserve(INIT_QUEUE_SIZE);
		}

		void push(const int id) {
			if (front >= stackSize) {
				stackSize += INIT_QUEUE_SIZE;
				IDs.reserve(stackSize);
			}
			IDs[front++] = id;
			empty = false;
		}

		int pop() {
			if (front - 1 == 0) empty = true;
			return (front != 0) ? IDs[--front] : -1;
		}

		[[nodiscard]] bool is_empty() const {
			return empty;
		}

		int num_elements() const {
			return front;
		}

		int size() const {
			return IDs.size();
		}

		int top() const {
			if (front == 0) return 0;
			return IDs[front - 1];
		}
	};
}
