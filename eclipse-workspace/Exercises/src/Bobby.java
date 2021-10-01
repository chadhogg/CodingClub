import java.util.Scanner;

//// Removed because Kattis won't have JUnit4 available to it.
//import org.junit.Test;
//import static org.junit.Assert.assertEquals;

import java.util.ArrayList;

/**
 * A solution to https://open.kattis.com/problems/bobby.
 * 
 * @author Chad Hogg
 */
public class Bobby {
	
	private static class Bet {
		/** The value needed to count a trial as a success. */
		public int R;
		/** The number of sides the die has. */
		public int S;
		/** The number of successes you need in order to win the bet. */
		public int X;
		/** The number of trials to do. */
		public int Y;
		/** The payment if you win the bet. */
		public int W;
		
		/**
		 * Constructs a new Bet.
		 * 
		 * @param r The value needed to count a trial as a success.
		 * @param s The number of sides the die has.
		 * @param x The number of successes needed.
		 * @param y The number of trials to run.
		 * @param w The payment if we get enough successes.
		 */
		public Bet(int r, int s, int x, int y, int w) {
			R = r;
			S = s;
			X = x;
			Y = y;
			W = w;
		}
		
		/**
		 * Decides whether or not this bet is worthwhile.
		 * 
		 * @return true if the expected value of the game is strictly greater than 1 (Bobby's cost to play).
		 */
		public boolean isWorthwhile() {
			double chanceOfSingleSuccess = (S - R + 1) / (double)S;
			double probOfWin = 1 - binomialCDF(X - 1, Y, chanceOfSingleSuccess);
			return probOfWin * W > 1;
		}
		
		/**
		 * Computes the cumulative distribution function for a binomial distribution.
		 * Algorithm taken from https://en.wikipedia.org/wiki/Binomial_distribution#Cumulative_distribution_function
		 * 
		 * @param k A number of successes.
		 * @param n A number of trials.
		 * @param p The probability of success in any given trial.
		 * @return The probability that the number of successes is *less than or equal to* k.
		 */
		public static double binomialCDF(int k, int n, double p) {
			double total = 0.0;
			for(int i = 0; i <= k; i++) {
				double nChooseI = choose(n, i);
				total += nChooseI * Math.pow(p, i) * Math.pow(1 - p, n - i);
			}
			return total;
		}
		
		/**
		 * Computes a binomial coefficient.
		 * Algorithm taken from https://en.wikipedia.org/wiki/Binomial_coefficient#Multiplicative_formula
		 * 
		 * @param n The top number.
		 * @param k The bottom number.
		 * @return The result of n choose k.
		 */
		public static double choose(double n, double k) {
			double product = 1.0;
			for(int i = 1; i <= k; i++) {
				product *= (n + 1 - i) / i;
			}
			return product;
		}
	}

	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		ArrayList<Bet> bets = new ArrayList<>();
		int numOfBets = console.nextInt();
		while(bets.size() < numOfBets) {
			bets.add(new Bet(console.nextInt(), console.nextInt(), console.nextInt(), console.nextInt(), console.nextInt()));
		}
		for(Bet bet : bets) {
			if(bet.isWorthwhile()) {
				System.out.println("yes");
			}
			else {
				System.out.println("no");
			}
		}
	}

////Removed because Kattis won't have JUnit4 available to it.
	/**
	 * Some tests because it seemed my math was off somewhere.
	 * Indeed, it was off in the choose method.  (My loop was running with <, not <=.)
	 */
	/*
	@Test
	public void testChoose() {
		final double EPSILON = 0.000001;
		assertEquals("Wrong value for 4 choose 2.", 6, Bet.choose(4, 2), EPSILON);    // Example from Wikipedia.
		assertEquals("Wrong value for 10 choose 1.", 10, Bet.choose(10, 1), EPSILON); // Worked out from definition.
		assertEquals("Wrong value for 20 choose 20.", 1, Bet.choose(20, 20), EPSILON);// Worked out from definition.
		assertEquals("Wrong value for 15 choose 3.", 455, Bet.choose(15, 3), EPSILON);// Asked WolframAlpha.
	}
	*/
}
