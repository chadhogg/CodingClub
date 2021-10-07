import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/speeding.
 * 
 * @author Chad Hogg
 */
public class Speeding {

	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 */
	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner console = new Scanner(System.in);
		int count = console.nextInt();
		int previousTime = console.nextInt();
		int previousDistance = console.nextInt();
		assert(previousTime == 0);
		assert(previousDistance == 0);
		int fastestSoFar = 0;
		for(int i = 1; i < count; i++) {
			assert(console.hasNextInt());
			int newTime = console.nextInt();
			assert(newTime > previousTime);
			assert(console.hasNextInt());
			int newDistance = console.nextInt();
			assert(newDistance > previousDistance);
			int averageSpeed = (newDistance - previousDistance) / (newTime - previousTime);
			if(averageSpeed > fastestSoFar) {
				fastestSoFar = averageSpeed;
			}
			previousTime = newTime;
			previousDistance = newDistance;
		}
		assert(!console.hasNextInt());
		System.out.println(fastestSoFar);
	}
}
