package fall2021;

import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/mathhomework.
 * 
 * @author Chad Hogg
 */
public class MathHomework {

	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner console = new Scanner(System.in);
		int firstLegs = console.nextInt();
		int secondLegs = console.nextInt();
		int thirdLegs = console.nextInt();
		int totalLegs = console.nextInt();
		boolean foundOne = false;
		for(int firstCount = 0; firstCount * firstLegs <= totalLegs; firstCount++) {
			int remainingLegs = totalLegs - (firstCount * firstLegs);
			for(int secondCount = 0; secondCount * secondLegs <= remainingLegs; secondCount++) {
				int stillRemainingLegs = remainingLegs - (secondCount * secondLegs);
				if(stillRemainingLegs % thirdLegs == 0) {
					int thirdCount = stillRemainingLegs / thirdLegs;
					System.out.println(firstCount + " " + secondCount + " " + thirdCount);
					foundOne = true;
				}
			}
		}
		if(!foundOne) {
			System.out.println("impossible");
		}
	}
}
