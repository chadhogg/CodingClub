package fall2021;
import java.util.PriorityQueue;
import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/birds
 * 
 * @author Chad Hogg
 */
public class Birds {
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 */
	public static void main(String[] args) {
		Problem problem = new Problem(new Scanner(System.in));
		System.out.println(solve(problem));
	}
	
	/**
	 * Finds the number of additional birds that can fit on a wire.
	 * Destroys the Problem in the process.
	 * 
	 * @param prob The problem to solve.
	 * @return The number of additional birds that can fit.
	 */
	private static long solve(Problem prob) {
		long total = 0;
		PriorityQueue<Long> queue = prob.birdPositions;
		long prevPos = queue.remove();
		while(!queue.isEmpty()) {
			long nextPos = queue.remove();
			long birdsInGap = (nextPos - prevPos - prob.DISTANCE_BETWEEN_BIRDS) / prob.DISTANCE_BETWEEN_BIRDS;
			total += birdsInGap;
			//System.out.println("Adding " + birdsInGap + " birds between " + prevPos + " and " + nextPos);
			prevPos = nextPos;
		}
		return total;
	}

	/**
	 * A problem to solve.
	 * 
	 * @author Chad Hogg
	 */
	private static class Problem {
		public static final long DISTANCE_FROM_POLES = 6;
		public final long WIRE_LENGTH;
		public final long DISTANCE_BETWEEN_BIRDS;
		public final long NUM_BIRDS;
		public PriorityQueue<Long> birdPositions;
		
		public Problem(Scanner console) {
			WIRE_LENGTH = console.nextLong();
			DISTANCE_BETWEEN_BIRDS = console.nextLong();
			NUM_BIRDS = console.nextLong();
			birdPositions = new PriorityQueue<>((int)(NUM_BIRDS + 2));
			for(long i = 0 ; i < NUM_BIRDS; i++) {
				birdPositions.add(console.nextLong());
			}
			// Add a dummy bird causing the first legal position to be at 6.
			birdPositions.add(DISTANCE_FROM_POLES - DISTANCE_BETWEEN_BIRDS);
			// Add a dummy bird causing the last legal position to be at length - 6.
			birdPositions.add(WIRE_LENGTH - (DISTANCE_FROM_POLES - DISTANCE_BETWEEN_BIRDS));
		}
	}
}
