package fall2021;
import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/apaxiaaans.
 * 
 * @author Chad Hogg
 */
public class Apaxiaaans {

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 */
	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner console = new Scanner(System.in);
		String input = console.next();
		StringBuilder output = new StringBuilder();
		output.append(input.charAt(0));
		char previous = input.charAt(0);
		for(int index = 1; index < input.length(); index++) {
			char current = input.charAt(index);
			if(previous != current) {
				output.append(current);
			}
			previous = current;
		}
		System.out.println(output.toString());
	}
}
