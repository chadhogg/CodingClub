package justforfun;

import java.util.Scanner;
import java.util.Set;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.TreeMap;

/**
 * An attempt at solving https://open.kattis.com/problems/paintball.
 * 
 * My first thought here was that this reduces to finding a Hamiltonian cycle in the player-canattack graph.
 * But that can't be correct, because finding one is NP-complete.
 * 
 * Then I realized that its more general than that -- we are looking for a set of Hamiltonian cycles such that each 
 *   vertex appears once somewhere in one of the cycles.
 * That's a less restrictive solution description, but it isn't clear to me that it makes the problem any easier.
 * 
 * So I decided to just try a greedy backtracking algorithm and hope for the best.
 * My greedy rule will be to choose the shooter with the fewest available targets or target with the fewest possible
 *    shooter; whichever is most constrained.
 * The goal here is to limit the branching factor of the search space as much as possible.
 * Unsurprisingly, this always provides correct answers but runs too slowly for some of the tests.
 * 
 * I tried just not backtracking and hoping that somehow any selection would work, but that gives us the wrong answer.
 * 
 * I next thought about going back to my original idea -- could I search for partitions of the graph into subgraphs,
 *   each of which has a Hamiltonian cycle?
 * But that still starts by deciding whether or not the entire graph has a Hamiltonian cycle, which is still too expensive.
 * 
 * On the advice of Will Killian, I tried using an adjacency matrix representation instead of the adjacency list while backtracking.
 * That was good advice, but now instead of timing out at test case 12 I am timing out at test case 18.
 * 
 * Finally, I gave up and looked at some other people's solutions.  I have a NewIdea class below based on those ideas, which is
 *    fast enough.
 * 
 * @author Chad Hogg
 *
 */
public class Paintball {

	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		Map<Integer, List<Integer>> adjacency = readInput(console);
		Map<Integer, Integer> result = NewIdea.findMapping(adjacency);
		if(result == null) {
			System.out.println("Impossible");
		}
		else {
			for(int attacker : result.keySet()) {
				System.out.println(result.get(attacker));
			}
		}
	}
	
	/**
	 * Creates an adjacency-list representation of the player-canshoot graph.
	 * 
	 * @param input A Scanner from which data can be read.
	 * @return A map of each player to the list of players they can shoot.
	 */
	private static Map<Integer, List<Integer>> readInput(Scanner input) {
		Map<Integer, List<Integer>> adjacency = new TreeMap<>();
		int numVertices = input.nextInt();
		int numEdges = input.nextInt();
		for(int i = 0; i < numVertices; i++) {
			adjacency.put(i + 1, new ArrayList<>());
		}
		for(int i = 0; i < numEdges; i++) {
			int a = input.nextInt();
			int b = input.nextInt();
			adjacency.get(a).add(b);
			adjacency.get(b).add(a);
		}
		return adjacency;
	}

	/**
	 * Converts an adjacency list representation to an adjacency matrix representation.
	 * 
	 * @param adjacencyList The adjacency list, in which the keys should be integers 1, 2, ... N.
	 * @return An adjacency matrix in which the first row/column are ignored.
	 */
	private static boolean[][] adjacencyListToMatrix(Map<Integer, List<Integer>> adjacencyList) {
		boolean[][] matrix = new boolean[adjacencyList.size() + 1][adjacencyList.size() + 1];
		for(int attacker : adjacencyList.keySet()) {
			List<Integer> list = adjacencyList.get(attacker);
			for(int target : list) {
				matrix[attacker][target] = true;
			}
		}
		return matrix;
	}
	
	/**
	 * Converts a solution in the form of an array to one in the form of a map.
	 * 
	 * @param solution An array in which positions 1, 2, ... N store the targets of those players.
	 * @return A map containing the same data.
	 */
	private static Map<Integer, Integer> solutionArrayToMap(int[] solution) {
		if(solution == null) {
			return null;
		}
		Map<Integer, Integer> map = new TreeMap<>();
		for(int i = 1; i < solution.length; i++) {
			map.put(i, solution[i]);
		}
		return map;
	}

	/**
	 * A simple object that stores a player number with a list of other player numbers.
	 * 
	 * @author Chad Hogg
	 */
	private static class PlayerWithAssociates {
		/** The number of the player. */
		public final int PLAYER;
		/** The numbers of other players associated with them in some way. */
		public List<Integer> associates;
		/** Whether the associates are people the player can attack (true) or people who can attack the player (false). */
		public final boolean ATTACK;
		
		/**
		 * Constructs a new object.
		 * 
		 * @param p The player.
		 * @param a Whether the player is the attacker (true) or target (false).
		 */
		public PlayerWithAssociates(int p, boolean a) {
			PLAYER = p;
			associates = new ArrayList<>();
			ATTACK = a;
		}
		
		@Override
		public String toString() {
			if(ATTACK) {
				return "player " + PLAYER + " to attack one of " + associates;
			}
			else {
				return "player " + PLAYER + " to be a target of one of " + associates;
			}
		}
	}

	/**
	 * A class that attempts to solve this problem with recursive backtracking.
	 * 
	 * Note that this approach works but proved too slow.
	 * (Not surprising, since it requires time exponential in the number of players.)
	 * 
	 * @author Chad Hogg
	 */
	@SuppressWarnings("unused")
	private static class BacktrackingSearcher {
		
		private static Set<Map<Integer, Integer>> failedPartialMappings = new HashSet<>();
		
		/**
		 * Searches for a way to assign each player one and only one target.
		 * 
		 * @param adjacency The adjacency list of the graph.
		 * @return A complete mapping of attacker to target, or null if none exists.
		 */
		public static Map<Integer, Integer> findMapping(Map<Integer, List<Integer>> adjacency) {
			Map<Integer, Integer> attackers = new HashMap<>();
			Map<Integer, Integer> targets = new HashMap<>();
			failedPartialMappings.clear();
			return findMapping(adjacency, attackers, targets);
		}
		
		/**
		 * Attempts to complete a partial mapping of attackers to targets.
		 * 
		 * @param adjacency An adjacency list representation of the graph.
		 * @param attackers A partial mapping of players to their targets.
		 * @param targets A partial mapping of targets to their attackers.
		 * @return A complete extension of the attackers map, or null if none exists.
		 */
		private static Map<Integer, Integer> findMapping(Map<Integer, List<Integer>> adjacency, Map<Integer, Integer> attackers, Map<Integer, Integer> targets) {
			if(attackers.size() == adjacency.size()) {
				return new TreeMap<>(attackers);
			}
			if(failedPartialMappings.contains(attackers)) {
				return null;
			}
			PlayerWithAssociates chosen = chooseMostConstrained(adjacency, attackers, targets);
			if(chosen.ATTACK) {
				for(int target : chosen.associates) {
					attackers.put(chosen.PLAYER, target);
					targets.put(target, chosen.PLAYER);
					Map<Integer, Integer> result = findMapping(adjacency, attackers, targets);
					if(result != null) {
						return result;
					}
					attackers.remove(chosen.PLAYER);
					targets.remove(target);
				}
			}
			else {
				for(int attacker : chosen.associates) {
					attackers.put(attacker, chosen.PLAYER);
					targets.put(chosen.PLAYER, attacker);
					Map<Integer, Integer> result = findMapping(adjacency, attackers, targets);
					if(result != null) {
						return result;
					}
					attackers.remove(attacker);
					targets.remove(chosen.PLAYER);
				}
			}
			failedPartialMappings.add(new HashMap<>(attackers));
			return null;
		}

		/**
		 * Chooses either the player with the fewest possible targets that are not yet being attacked,
		 *   or the player with the fewest possible attackers who are not yet attacking someone else.
		 *   
		 * @param adjacency The adjacency list of the graph.
		 * @param attackers A partial mapping of players to the people they are attacking.
		 * @param targets A partial mapping of players to the people who are attacking them.
		 * @return The most constrained player, with their set of options.
		 */
		private static PlayerWithAssociates chooseMostConstrained(Map<Integer, List<Integer>> adjacency, Map<Integer, Integer> attackers, Map<Integer, Integer> targets) {
			PlayerWithAssociates mostConstrained = null;
			for(int possibleAttacker : adjacency.keySet()) {
				if(!attackers.containsKey(possibleAttacker)) {
					PlayerWithAssociates thisGuy = new PlayerWithAssociates(possibleAttacker, true);
					for(Integer possibleTarget : adjacency.get(possibleAttacker)) {
						if(!targets.containsKey(possibleTarget)) {
							thisGuy.associates.add(possibleTarget);
						}
					}
					if(mostConstrained == null || thisGuy.associates.size() < mostConstrained.associates.size()) {
						mostConstrained = thisGuy;
						if(mostConstrained.associates.isEmpty()) {
							return mostConstrained;
						}
					}
				}
			}
			for(int possibleTarget : adjacency.keySet()) {
				if(!targets.containsKey(possibleTarget)) {
					PlayerWithAssociates thisGuy = new PlayerWithAssociates(possibleTarget, false);
					for(int possibleAttacker : adjacency.keySet()) {
						if(!attackers.containsKey(possibleAttacker) && adjacency.get(possibleAttacker).contains(possibleTarget)) {
							thisGuy.associates.add(possibleAttacker);
						}
					}
					if(mostConstrained == null || thisGuy.associates.size() < mostConstrained.associates.size()) {
						mostConstrained = thisGuy;
						if(mostConstrained.associates.isEmpty()) {
							return mostConstrained;
						}
					}
				}
			}
			//System.out.println("Adjacency: " + adjacency);
			//System.out.println("Attackers: " + attackers);
			//System.out.println("Targets: " + targets);
			//System.out.println("I chose " + mostConstrained);
			return mostConstrained;
		}
		
	}
		
	/**
	 * This is intended to be just a reimplementation of BacktrackingSearcher that stores the graph in an adjacency matrix instead.
	 * Will Killian suggests that it will be much faster to access even though it is quite sparse.
	 * 
	 * @author Chad Hogg
	 */
	@SuppressWarnings("unused")
	private static class MatrixBacktrackingSearcher {

		private static Set<int[]> failedPartialMappings = new HashSet<>();
		
		/**
		 * Searches for a way to assign each player one and only one target.
		 * 
		 * @param adjacency The adjacency list of the graph.
		 * @return A complete mapping of attacker to target, or null if none exists.
		 */
		public static Map<Integer, Integer> findMapping(Map<Integer, List<Integer>> adjacency) {
			failedPartialMappings.clear();
			boolean[][] matrix = adjacencyListToMatrix(adjacency);
			int[] result =  findMapping(matrix, new int[adjacency.size() + 1], new int[adjacency.size() + 1]);
			return solutionArrayToMap(result);
		}
		
		private static boolean allButFirstNonZero(int[] array) {
			for(int i = 1; i < array.length; i++) {
				if(array[i] == 0) {
					return false;
				}
			}
			return true;
		}

		/**
		 * Attempts to complete a partial mapping of attackers to targets.
		 * 
		 * @param adjacency An adjacency matrix representation of the graph.
		 * @param attackers A partial mapping of players to their targets.
		 * @param targets A partial mapping of targets to their attackers.
		 * @return A complete extension of the attackers map, or null if none exists.
		 */
		private static int[] findMapping(boolean[][] adjacency, int[] attackers, int[] targets) {
			if(allButFirstNonZero(attackers)) {
				return attackers;
			}
			if(failedPartialMappings.contains(attackers)) {
				return null;
			}
			PlayerWithAssociates chosen = chooseMostConstrained(adjacency, attackers, targets);
			if(chosen.ATTACK) {
				for(int target : chosen.associates) {
					attackers[chosen.PLAYER] = target;
					targets[target] = chosen.PLAYER;
					int[] result = findMapping(adjacency, attackers, targets);
					if(result != null) {
						return result;
					}
					attackers[chosen.PLAYER] = 0;
					targets[target] = 0;
				}
			}
			else {
				for(int attacker : chosen.associates) {
					attackers[attacker] = chosen.PLAYER;
					targets[chosen.PLAYER] = attacker;
					int[] result = findMapping(adjacency, attackers, targets);
					if(result != null) {
						return result;
					}
					attackers[attacker] = 0;
					targets[chosen.PLAYER] = 0;
				}
			}
			failedPartialMappings.add(Arrays.copyOf(attackers, attackers.length));
			return null;
		}

		/**
		 * Chooses either the player with the fewest possible targets that are not yet being attacked,
		 *   or the player with the fewest possible attackers who are not yet attacking someone else.
		 *   
		 * @param adjacency The adjacency matrix of the graph.
		 * @param attackers A partial mapping of players to the people they are attacking.
		 * @param targets A partial mapping of players to the people who are attacking them.
		 * @return The most constrained player, with their set of options.
		 */
		private static PlayerWithAssociates chooseMostConstrained(boolean[][] adjacency, int[] attackers, int[] targets) {
			PlayerWithAssociates mostConstrained = null;
			for(int possibleAttacker = 1; possibleAttacker < adjacency.length; possibleAttacker++) {
				if(attackers[possibleAttacker] == 0) {
					PlayerWithAssociates thisGuy = new PlayerWithAssociates(possibleAttacker, true);
					for(int possibleTarget = 1; possibleTarget < adjacency.length; possibleTarget++) {
						if(adjacency[possibleAttacker][possibleTarget] && targets[possibleTarget] == 0) {
							thisGuy.associates.add(possibleTarget);
						}
					}
					if(mostConstrained == null || thisGuy.associates.size() < mostConstrained.associates.size()) {
						mostConstrained = thisGuy;
						if(mostConstrained.associates.isEmpty()) {
							return mostConstrained;
						}
					}
				}
			}
			for(int possibleTarget = 1; possibleTarget < adjacency.length; possibleTarget++) {
				if(targets[possibleTarget] == 0) {
					PlayerWithAssociates thisGuy = new PlayerWithAssociates(possibleTarget, false);
					for(int possibleAttacker = 1; possibleAttacker < adjacency.length; possibleAttacker++) {
						if(adjacency[possibleAttacker][possibleTarget] && attackers[possibleAttacker] == 0) {
							thisGuy.associates.add(possibleAttacker);
						}
					}
					if(mostConstrained == null || thisGuy.associates.size() < mostConstrained.associates.size()) {
						mostConstrained = thisGuy;
						if(mostConstrained.associates.isEmpty()) {
							return mostConstrained;
						}
					}
				}
			}
			//System.out.println("Adjacency: " + adjacency);
			//System.out.println("Attackers: " + attackers);
			//System.out.println("Targets: " + targets);
			//System.out.println("I chose " + mostConstrained);
			return mostConstrained;
		}

	}
	
	/**
	 * An algorithm inspired by https://github.com/hliuliu/kattis_problems/blob/master/paintball/paintball.java
	 * 
	 * I have to admit, I still don't entirely understand how it works.
	 * The key is that we can remap a previously-assigned attacker to a new target, a bounded number of times.
	 * 
	 * @author hliuliu, Chad Hogg
	 */
	private static class NewIdea {
		/** An adjacency matrix for the players (1 ... N). */
		private static boolean[][] adjacencyMatrix;
		/** A mapping of player who is making an attack (1 ... N) to player being hit (1 ... N). */
		private static int[] attackerToTarget;
		/** A mapping of player who is being hit (1 ... N) to player attacking them (1 ... N). */
		private static int[] targetToAttacker;
		/** Whether or not we have considered a player during the current processing step. */
		private static boolean[] visited;
		
		/**
		 * Try to find a mapping of attackers to targets.
		 * 
		 * @param adjacencyList An adjacency-list representation of the graph.
		 * @return A mapping of attackers to targets, or null if none exists.
		 */
		public static Map<Integer, Integer> findMapping(Map<Integer, List<Integer>> adjacencyList) {
			adjacencyMatrix = adjacencyListToMatrix(adjacencyList);
			// These start with 0s, which are sentinel values for "not yet set".
			attackerToTarget = new int[adjacencyMatrix.length];
			targetToAttacker = new int[adjacencyMatrix.length];
			int count = 0;
			// Each potential attacker must be given a target.
			for(int attacker = 1; attacker < attackerToTarget.length; attacker++) {
				// So far we haven't visited any node when assigning this attacker.
				visited = new boolean[adjacencyMatrix.length];
				// Find a target for this attacker, if possible.
				if(tryToMap(attacker)) {
					count++;
				}
			}
			if(count == adjacencyList.size()) {
				return solutionArrayToMap(attackerToTarget);
			}
			else {
				return null;
			}
		}
		
		/**
		 * Attempts to find a target for a specific attacker.
		 * 
		 * @param attacker The attacker.
		 * @return Whether or not it was possible to find a target.
		 */
		private static boolean tryToMap(int attacker) {
			// Consider all possible targets.
			for(int possibleTarget = 1; possibleTarget < adjacencyMatrix.length; possibleTarget++) {
				// It's only worth attempting a target if the attacker can see them and we haven't yet visited it.
				if(adjacencyMatrix[attacker][possibleTarget] && !visited[possibleTarget]) {
					// Now we have visited it.
					visited[possibleTarget] = true;
					// If no one is yet attacking the target, this attacker can!
					if(targetToAttacker[possibleTarget] == 0) {
						attackerToTarget[attacker] = possibleTarget;
						targetToAttacker[possibleTarget] = attacker;
						return true;
					}
					// If I can find a different target for the person currently attacking them, we can steal this target.
					// Only potential targets that haven't been visited yet will be considered, which limits this from spiraling
					//   into exponential or infinite time.
					else if(tryToMap(targetToAttacker[possibleTarget])) {
						attackerToTarget[attacker] = possibleTarget;
						targetToAttacker[possibleTarget] = attacker;
						return true;						
					}
				}
			}
			return false;
		}
	}
}
