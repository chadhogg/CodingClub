package fall2021;

import java.util.Random;

/**
 * A program that calculates the expected number of people who will successfully cross a bridge given that:
 *   - There are initially 2 options at each choice point.
 *   - One option at each choice point is a hidden failure.
 *   - Once anyone discovers a failure point, all future people know about it.
 *   
 * @author Chad Hogg
 */
public class Squiddler {

	/** The number of places a decision must be made. */
	public static final int BRIDGE_LENGTH = 18;
	/** The number of people who will attempt to walk across the bridge. */
	public static final int TOTAL_CONTESTANTS = 16;
	/** A pseudo-random number generator. */
	private static Random rand = new Random();
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		System.out.println("Monte Carlo approximation: " + MonteCarloSimulation.simulateMany());
	}
	
	/**
	 * A class that simulates many instances of the puzzle, observing what actually happens, and averaging the results.
	 * 
	 * @author Chad Hogg
	 */
	private static class MonteCarloSimulation {
		
		/**
		 * Runs many simulations, reporting the average result.
		 * 
		 * @return The average number of people that make it across the bridge.
		 */
		public static double simulateMany() {
			final int ATTEMPTS = 1000000;
			double total = 0;
			for(int x = 0; x < ATTEMPTS; x++) {
				total += simulateOne();
			}
			return total / ATTEMPTS;
		}
		
		/**
		 * Simulates one game.
		 * 
		 * @return The number of people who make it off the bridge in this game.
		 */
		private static int simulateOne() {
			int peeps = 0;
			int numDiscovered = 0;
			// Each contestant attempts to go across.
			for(int i = 0; i < TOTAL_CONTESTANTS; i++) {
				// If prior contestants have already revealed the safe choice at every position, they get across.
				if(numDiscovered == BRIDGE_LENGTH) {
					peeps++;
				}
				else {
					boolean heDead = false;
					// This contestant gets through all of the already-figured-out positions without difficulty,
					//   then continues until reaching the end or dying.
					for(int j = numDiscovered; j < BRIDGE_LENGTH && !heDead; j++) {
						// Attempting a section of the bridge makes the correct choice known for future contestants.
						numDiscovered++;
						// You ... have chosen poorly.
						if(rand.nextBoolean()) {
							heDead = true;
						}
					}
					// If they made it across, they count!
					if(!heDead) {
						peeps++;
					}
				}
			}
			return peeps;
		}
	}
}
