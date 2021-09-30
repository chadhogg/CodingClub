import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/missingnumbers
 * 
 * Accepts a monotonically increasing list of integers from standard input.
 * Prints any missing integers to stdout, or "good job" if none are missing.
 * 
 * @author Chad Hogg
 *
 */
public class MissingNumbers {

	/**
	 * Runs the program.
	 * 
	 * @param args Ignored.
	 */
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		// The number of (additional) inputs to read.
		int howMany = console.nextInt();
		// Whether or not there were any missing numbers.
		boolean missedOne = false;
		// The next number I expect the user to enter.
		int expected = 1;
		// Read the number of inputs I was told there would be.
		for(int count = 0; count < howMany; count++) {
			// Read this input.
			int thisOne = console.nextInt();
			// The user might have skipped a lot; print them all.
			while(thisOne != expected) {
				System.out.println(expected);
				missedOne = true;
				expected++;
			}
			// Now I expect the following number.
			expected++;
		}
		// Handle this special case.
		if(!missedOne) {
			System.out.println("good job");
		}
	}
}
