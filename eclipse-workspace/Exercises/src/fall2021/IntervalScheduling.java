package fall2021;

import java.util.Collections;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;

/**
 * A solution to https://open.kattis.com/problems/intervalscheduling.
 * 
 * The key insight here is that because all intervals are of equal value, it is always "safe" to choose, among all
 *   still schedulable intervals, the one that ends first.  This is safe because it leaves the maximum amount of time
 *   in which additional intervals can be scheduled.
 * Or, put another way, the number of conflicts the interval that ends earliest has with other still schedulable intervals
 *    cannot be higher than the number of conflicts that other still schedulable intervals that it conflicts with have.
 * 
 * @author Chad Hogg
 */
public class IntervalScheduling {
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		Problem prob = new Problem(console);
		System.out.println(findOptimalSchedule(prob).size());
	}
	
	/**
	 * Chooses the largest possible set of intervals that can be simultaneously scheduled.
	 * 
	 * @param prob The problem to solve.
	 * @return The (not necessarily unique) largest possible of set of simultaneously-schedulable intervals from the problem.
	 */
	private static Set<Interval> findOptimalSchedule(Problem prob) {
		SortedSet<Interval> available = new TreeSet<>(prob.INTERVALS);
		int earliestAvailableStartTime = 0;
		Set<Interval> chosen = new TreeSet<Interval>();
		for(Interval next : available) {
			if(next.START >= earliestAvailableStartTime) {
				chosen.add(next);
				earliestAvailableStartTime = next.END;
			}
		}
		return chosen;
	}

	/**
	 * An interval of time in which some resource would be in use.
	 * 
	 * @author Chad Hogg
	 */
	private static class Interval implements Comparable<Interval> {
	
		/** The time at which this interval begins. */
		public final int START;

		/** The time at which this interval ends. */
		public final int END;
		
		/**
		 * Constructs a new interval.
		 * 
		 * @param s When the new interval starts.
		 * @param e When the new interval ends.
		 */
		public Interval(int s, int e) {
			START = s;
			END = e;
		}
		
		/**
		 * Gets the length of this interval.
		 * 
		 * @return The difference in end and start times.
		 */
		public int length() {
			return END - START;
		}
		
		@Override
		public boolean equals(Object obj) {
			if(obj == null) { return false; }
			if(this.getClass() != obj.getClass()) { return false; }
			Interval other = (Interval)obj;
			return this.START == other.START && this.END == other.END;
		}
		
		@Override
		public int hashCode() {
			return START * 454521 + END;
		}
		
		@Override
		public String toString() {
			return "(" + START + " -> " + END + ")";
		}
		
		/**
		 * Compares intervals in such a way that intervals that end earlier come first, ties broken to shorter intervals first.
		 * 
		 * @param other Another interval.
		 * @return Negative is this interval comes earlier, positive if the other comes earlier, zero if identical. 
		 */
		@Override
		public int compareTo(Interval other) {
			int endDiff = this.END - other.END;
			if(endDiff == 0) {
				return this.length() - other.length();
			}
			return endDiff;
		}
	}

	/**
	 * An immutable problem to solve.
	 * 
	 * @author Chad Hogg
	 */
	private static class Problem {
		/** The set of intervals. */
		public final Set<Interval> INTERVALS;
		
		/**
		 * Constructs a problem.
		 * 
		 * @param input A Scanner from which the problem details can be read.
		 */
		public Problem(Scanner input) {
			Set<Interval> intervals = new HashSet<>();
			int num = input.nextInt();
			for(int i = 0 ; i < num; i++) {
				intervals.add(new Interval(input.nextInt(), input.nextInt()));
			}
			INTERVALS = Collections.unmodifiableSet(intervals);
		}
	}
}
