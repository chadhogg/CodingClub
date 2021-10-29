package fall2021;
import java.util.ArrayList;
import java.util.NoSuchElementException;

/**
 * A program that tests a method for finding the kth-smallest element in an array, in O(n log k) time.
 * 
 * Justification of computational complexity:
 *   Terms:
 *     n: The length of the array.
 *     k: Which number we are looking for.
 *   The loop through the array runs n times, so whatever happens inside the loop gets multiplied by O(n).
 *   Most of the steps in the loop run in constant time.
 *   Adding to / removing from the heap does not run in constant time, 
 *     but instead in O(log k) time (k being the maximum size of the heap).
 *   Thus, we do (up to) 2 O(log k) operations O(n) times, leading to O(n log k) time.
 * 
 * @author Chad Hogg
 */
public class KthSmallest {
	
	/**
	 * Runs a simple test.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		int[] numbers = { 8, 6, 2, 19, -12, 7, 4, 10};
		int k = 3;
		System.out.println(k + "-th smallest is " + kthSmallest(numbers, k));
	}
	
	/**
	 * Finds the kth-smallest number in an array.
	 * 
	 * @param numbers The array of numbers.
	 * @param k Which number we want.  Must be <= numbers.length.
	 * @return The kth-smallest number from the array.
	 */
	private static int kthSmallest(int[] numbers, int k) {
		if(k > numbers.length) {
			throw new IllegalArgumentException("Not enough numbers.");
		}
		// This makes a MAX-heap, which seems weird when we are looking for low numbers, but is appropriate here.
		MUHeap<Integer> heap = new MUHeap<>(true);
		for(int num : numbers) {
			if(heap.size() < k) {
				heap.add(num);
			}
			else {
				// The largest in the heap is the kth-smallest of the numbers we have processed so far.
				int largestInHeap = heap.remove();
				if(num < largestInHeap) {
					// The new number should replace it in the heap.
					heap.add(num);
				}
				else {
					// It should stay in the heap.
					heap.add(largestInHeap);
				}
			}
		}
		return heap.remove();
	}

	/**
	 * A binary tree that must be complete and have children not larger (max) / smaller (min) than their parents.
	 * 
	 * NOTE: I stole this helper class from something I wrote for CSCI162.
	 * 
	 * @author Chad Hogg
	 *
	 * @param <E> The type of element stored in the heap, which must be comparable to itself.
	 */
	private static class MUHeap<E extends Comparable<E>> {
		
		// Class invariants:
		//   1: This represents a binary tree that is complete.
		//   2: This represents a binary tree in which no child is larger (max) / smaller (min) than its parent.
		//   3: If there are N elements in the heap, they are stored at indexes 0 through N-1 in the list.
		//   4: The overall root of the heap is stored at index 0 in the list.
		//   5: For any node in the list, if i is the index of that node, 2i+1 is the index of its left child.
		//   6: For any node in the list, if i is the index of that node, 2i+2 is the index of its right child.
		

		/** A list containing the element in level order. */
		private ArrayList<E> list;
		/** Whether this is a max-heap (true) or min-heap (false). */
		private boolean isMax;
		
		/**
		 * Creates a new, empty heap.
		 * 
		 * @param isMax Whether this should be a max-heap (true) or min-heap (false).
		 */
		public MUHeap(boolean isMax) {
			list = new ArrayList<>();
			this.isMax = isMax;
		}
		
		///// Generally useful helper methods. /////

		/**
		 * Gets the index where the parent of a node would be.
		 * 
		 * Note that the node at index 0 does not have a parent, so you shouldn't call this method on it.
		 * @param index The index of the child node.
		 * @return The index of the parent node.
		 */
		private int getParentIndex(int index) {
			return (index - 1) / 2;
		}
		
		/**
		 * Gets the index where the left child of a node would be.
		 * 
		 * @param index The index of the parent node.
		 * @return The index of its left child.
		 */
		private int getLeftChildIndex(int index) {
			return 2 * index + 1;
		}
		
		/**
		 * Gets the index where the right child of a node would be.
		 * 
		 * @param index The index of the parent node.
		 * @return The index of its right child.
		 */
		private int getRightChildIndex(int index) {
			return 2 * index + 2;
		}
		
		/**
		 * Gets whether or not an index contains an element.
		 * 
		 * @param index The index of a potential node.
		 * @return True if there is a node there, false if it is null.
		 */
		private boolean indexHasNode(int index) {
			return index < list.size();
		}
		
		/**
		 * Gets the index where the next element should be (temporarily) inserted.
		 * 
		 * @return The index of the first un-used spot in the list.
		 */
		private int getNextInsertionPoint() {
			return list.size();
		}
		
		/**
		 * Gets the index where the next element should be (temporarily) moved to the top when removing.
		 * 
		 * @return The index of the last used spot in the list.
		 */
		private int getNextRemovalPoint() {
			return list.size() - 1;
		}
		
		/**
		 * Decides whether or not one element can be above another in the tree.
		 * 
		 * @param index1 The index of the first element.
		 * @param index2 The index of the second element.
		 * @return True if the element at index1 should be above the element at index2.
		 */
		private boolean firstCanBeAbove(int index1, int index2) {
			if(isMax) {
				return list.get(index1).compareTo(list.get(index2)) >= 0;
			}
			else {
				return list.get(index1).compareTo(list.get(index2)) <= 0;
			}
		}

		/**
		 * Swaps the values stored at two different array indices.
		 * 
		 * @param firstIndex The first index, which must be valid.
		 * @param secondIndex The second index, which must be valid.
		 * @postcondition array[firstIndex] contains what was at array[secondIndex] and vice versa.
		 * @throws IndexOutOfBoundsException If an index is invalid.
		 */
		private void swap(int firstIndex, int secondIndex) {
			E temp = list.get(firstIndex);
			list.set(firstIndex, list.get(secondIndex));
			list.set(secondIndex, temp);
		}

		//// Methods for converting to string ////
		
		/**
		 * Gets the number of characters in the string representation of the largest element in this tree.
		 * 
		 * @return The width of the widest element, or 0 if there are no elements.
		 */
		private int widthOfWidestElement() {
			int width = 0;
			for(int index = 0; index < list.size(); index++) {
				if(list.get(index).toString().length() > width) {
					width = list.get(index).toString().length();
				}
			}
			return width;
		}
		
		/**
		 * Calculates an integer raised to a power.
		 * 
		 * @param base The number that will be raised to a power.
		 * @param exponent The power, which may not be negative.
		 * @return The base raised to the exponent power.
		 * @throws IllegalArgumentException If the exponent is negative.
		 */
		private static int pow(int base, int exponent) {
			int result;
			if(exponent < 0) {
				throw new IllegalArgumentException("Cannot raise base to negative exponent.");
			}
			else if(exponent == 0) {
				result = 1;
			}
			else if(exponent % 2 == 0) {
				result = pow(base * base, exponent / 2);
			}
			else {
				result = base * pow(base, exponent - 1);
			}
			return result;
		}

		/**
		 * Appends some number of copies of a String to a StringBuilder.
		 * 
		 * @param builder The StringBuilder, which may not be null.
		 * @param toAdd The String to append, which may not be null.
		 * @param copies The number of times to append, which may not be negative.
		 * @postcondition The toAdd String has been appended to builder copies number of times.
		 * @throws IllegalArgumentException If copies is negative.
		 */
		public static void appendCopies(StringBuilder builder, String toAdd, int copies) {
			if(copies < 0) {
				throw new IllegalArgumentException("Number of copies may not be null.");
			}
			for(int counter = 0; counter < copies; counter++) {
				builder.append(toAdd);
			}
		}
		
		/**
		 * Appends a String to a StringBuilder, center-justified in a certain field width.
		 * 
		 * @param builder The StringBuilder to which things will be appended, which may not be null.
		 * @param toAdd The String to append, which may not be null.
		 * @param fieldWidth The desired width of the field, which may not be negative.
		 * @postcondition toAdd has been appended to builder, as well as enough spaces on either side to
		 *   take up at least fieldWidth characters.
		 * @throws IllegalArgumentException If fieldWidth is negative.
		 */
		private static void appendCenter(StringBuilder builder, String toAdd, int fieldWidth) {
			int missingSpaces = fieldWidth - toAdd.length();
			appendCopies(builder, " ", missingSpaces / 2);
			builder.append(toAdd);
			appendCopies(builder, " ", (missingSpaces + 1) / 2);
		}
		
		/**
		 * Builds a picture of the tree.
		 * 
		 * @param spacing Half the desired width of each element in the tree.
		 * @return A String containing a picture of the tree.
		 */
		private String wholeTree(int spacing) {
			StringBuilder builder = new StringBuilder();
			// Create a list of levels, each of which is a list of the elements in that level.
			ArrayList<ArrayList<E>> levels = new ArrayList<ArrayList<E>>();
			for(int index = 0; index < list.size(); index++) {
				if(levels.isEmpty() || levels.get(levels.size() - 1).size() == pow(2, levels.size() - 1)) {
					levels.add(new ArrayList<E>());
				}
				levels.get(levels.size() - 1).add(list.get(index));
			}
			// Add each level to the picture, from top to bottom.
			for(int levelNumber = 0; levelNumber < levels.size(); levelNumber++) {
				// This formula required a lot of drawing pictures and trying things to derive.
				int spacesBefore = (spacing) * (pow(2, levels.size() - levelNumber - 1) - 1);
				int spacesBetween = spacesBefore * 2;
				// Add each element in this level to the picture, from left to right.
				for(int nodeNumber = 0; nodeNumber < levels.get(levelNumber).size(); nodeNumber++) {
					// Apply some padding before the element to center parents between children.
					if(nodeNumber == 0) {
						appendCopies(builder, " ", spacesBefore);
					}
					else {
						appendCopies(builder, " ", spacesBetween);					
					}
					appendCenter(builder, levels.get(levelNumber).get(nodeNumber).toString(), spacing * 2);
				}
				builder.append("\n");
			}
			return builder.toString();
		}
		
		/**
		 * Gets a String representation of this heap.
		 * 
		 * @return A picture of the tree.
		 */
		@Override
		public String toString() {
			return wholeTree(widthOfWidestElement() + 1);
		}

		
		//// Methods for checking heap property ////
		
		/**
		 * Checks whether or not the heap property holds on the entire tree.
		 * 
		 * @return True if no parent in the tree is out of order with its children; false otherwise.
		 */
		private boolean hasHeapProperty() {
			if(isEmpty()) {
				return true;
			}
			else {
				return hasHeapProperty(0);
			}
		}
		
		/**
		 * Checks whether or not the heap property holds on some subtree.
		 * Note that if this returns false, an important class invariant has been violated.
		 * 
		 * @param subtreeRoot The index of that subtree's root, which must be valid.
		 * @return True if no parent in that subtree is out of order with its children; false otherwise.
		 */
		private boolean hasHeapProperty(int subtreeRoot) {
			int leftChildIndex = getLeftChildIndex(subtreeRoot);
			int rightChildIndex = getRightChildIndex(subtreeRoot);
			if(indexHasNode(leftChildIndex)) {
				if(!firstCanBeAbove(subtreeRoot, leftChildIndex) || !hasHeapProperty(leftChildIndex)) {
					return false;
				}
			}
			if(indexHasNode(rightChildIndex)) {
				if(!firstCanBeAbove(subtreeRoot, rightChildIndex) || !hasHeapProperty(rightChildIndex)) {
					return false;
				}
			}
			return true;
		}

		//// Actual public methods, with helpers unique to them ////
		
		/**
		 * Gets the number of elements in this heap.
		 * 
		 * @return The size of this heap.
		 */
		public int size() {
			return list.size();
		}
		
		/**
		 * Gets whether or not this heap is empty.
		 * 
		 * @return True if size is 0; false otherwise.
		 */
		public boolean isEmpty() {
			return list.isEmpty();
		}

		/**
		 * Adds a new element to this heap.
		 * 
		 * @param element The element to insert.
		 * @postcondition This heap contains one more copy of that element than it previously did.
		 */
		public void add(E element) {
			if(!hasHeapProperty()) {
				throw new IllegalStateException("Does not have heap property.");
			}
			int index = getNextInsertionPoint();
			list.add(element);
			reheapifyUp(index);
			if(!hasHeapProperty()) {
				throw new IllegalStateException("Does not have heap property.");
			}
		}
		
		/**
		 * Swaps an elements with its ancestors until it runs out of ancestors or finds one
		 *   larger than it.
		 *   
		 * @param index The index of the element to heapify up, which must be valid.
		 * @postcondition If the specified index was the only one that violated the heap property, the
		 *   entire thing now satisfies the heap property and contains the same elements.
		 */
		private void reheapifyUp(int index) {
			if(index > 0) {
				int parentIndex = getParentIndex(index);
				if(firstCanBeAbove(index, parentIndex)) {
					swap(index, parentIndex);
					reheapifyUp(parentIndex);
				}
			}
		}
		
		/**
		 * Removes and returns the largest (max) or smallest (min) element of this heap.
		 * 
		 * @precondition This heap is not empty.
		 * @postcondition The largest/smallest element of this heap has been removed.
		 * @return The element that was removed.
		 */
		public E remove() {
			if(list.size() == 0) {
				throw new NoSuchElementException();
			}
			if(!hasHeapProperty()) {
				throw new IllegalStateException("Does not have heap property.");
			}		
			E largest = list.get(0);
			list.set(0, list.get(getNextRemovalPoint()));
			list.remove(getNextRemovalPoint());
			reheapifyDown(0);
			if(!hasHeapProperty()) {
				throw new IllegalStateException("Does not have heap property.");
			}
			return largest;
		}

		/**
		 * Repeatedly swaps an element with its most extreme child, until it has no child that should be above itself.
		 * 
		 * @param index The index of the element to reheapify down, which must be valid.
		 * @postcondition If the specified element was the only one that violated the heap property, then
		 *   that property now holds for the entire tree, which still contains the same elements.
		 */
		private void reheapifyDown(int index) {
			int leftChildIndex = getLeftChildIndex(index);
			int rightChildIndex = getRightChildIndex(index);
			if(indexHasNode(leftChildIndex)) {
				if(indexHasNode(rightChildIndex)) {
					boolean leftHigher = firstCanBeAbove(leftChildIndex, rightChildIndex);
					if(leftHigher) {
						if(firstCanBeAbove(leftChildIndex, index)) {
							swap(index, leftChildIndex);
							reheapifyDown(leftChildIndex);
						}
					}
					else {
						if(firstCanBeAbove(rightChildIndex, index)) {
							swap(index, rightChildIndex);
							reheapifyDown(rightChildIndex);
						}
					}
				}
				else {
					if(firstCanBeAbove(leftChildIndex, index)) {
						swap(index, leftChildIndex);
						reheapifyDown(leftChildIndex);
					}
				}
			}
		}
		

	}

}
