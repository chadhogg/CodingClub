package fall2021;

import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/mastermind.
 * 
 * @author Chad Hogg
 */
public class Mastermind {

	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner console = new Scanner(System.in);
		int length = console.nextInt();
		String code = console.next();
		String guess = console.next();
		boolean[] codeMatched = new boolean[length];
		boolean[] guessMatched = new boolean[length];
		
		int perfectMatches = 0;
		int partialMatches = 0;
		for(int index = 0; index < length; index++) {
			if(code.charAt(index) == guess.charAt(index)) {
				perfectMatches++;
				codeMatched[index] = true;
				guessMatched[index] = true;
			}
		}
		for(int guessIndex = 0; guessIndex < length; guessIndex++) {
			for(int codeIndex = 0; codeIndex < length && !guessMatched[guessIndex]; codeIndex++) {
				if(!codeMatched[codeIndex] && code.charAt(codeIndex) == guess.charAt(guessIndex)) {
					partialMatches++;
					codeMatched[codeIndex] = true;
					guessMatched[guessIndex] = true;
				}
			}
		}
		System.out.println(perfectMatches + " " + partialMatches);
	}
}
