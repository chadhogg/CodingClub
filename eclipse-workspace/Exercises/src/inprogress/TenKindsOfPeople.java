package inprogress;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;
import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;
import java.util.PriorityQueue;

/**
 * A solution to https://open.kattis.com/problems/10kindsofpeople
 * Note that we didn't actually do this in Coding Club, I just decided to do it for fun.
 * 
 * Also note: This is currently too slow, and I need to come back to it.
 * 
 * @author Chad Hogg
 */
public class TenKindsOfPeople {

	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		Problem problem = new Problem(console);
		List<String> solutions = solveAllQueriesSimultaneously(problem);
		for(String solution : solutions) {
			System.out.println(solution);
		}
	}
	
	private static List<String> solveAllQueriesSimultaneously(Problem problem) {
		List<String> answers = new ArrayList<>(problem.QUERIES.size());
		List<Set<Pos>> connectedComponents = new ArrayList<>();
		Set<Pos> independentPositions = new HashSet<>();
		for(int i = 0; i < problem.ROWS; i++) {
			for(int j = 0; j < problem.COLS; j++) {
				independentPositions.add(new Pos(i, j));
			}
		}
		while(!independentPositions.isEmpty()) {
			Set<Pos> newComponent = new HashSet<>();
			Pos pos = removeArbitraryElement(independentPositions);
			Set<Pos> frontier = new HashSet<>();
			frontier.add(pos);
			while(!frontier.isEmpty()) {
				Pos current = removeArbitraryElement(frontier);
				if(current.ROW > 0) {
					if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW - 1][current.COL]) {
						Pos north = new Pos(current.ROW - 1, current.COL);
						if(!newComponent.contains(north) && !frontier.contains(north)) {
							frontier.add(north);
							independentPositions.remove(north);
						}
					}
				}
				if(current.ROW < problem.ROWS - 1) {
					if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW + 1][current.COL]) {
						Pos south = new Pos(current.ROW + 1, current.COL);
						if(!newComponent.contains(south) && !frontier.contains(south)) {
							frontier.add(south);
							independentPositions.remove(south);
						}
					}
				}
				if(current.COL > 0) {
					if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW][current.COL - 1]) {
						Pos west = new Pos(current.ROW, current.COL - 1);
						if(!newComponent.contains(west) && !frontier.contains(west)) {
							frontier.add(west);
							independentPositions.remove(west);
						}
					}
				}
				if(current.COL < problem.COLS - 1) {
					if(problem.MAP[current.ROW][current.COL]== problem.MAP[current.ROW][current.COL + 1]) {
						Pos east = new Pos(current.ROW, current.COL + 1);
						if(!newComponent.contains(east) && !frontier.contains(east)) {
							frontier.add(east);
							independentPositions.remove(east);
						}
					}
				}
				newComponent.add(current);
			}
			
			connectedComponents.add(newComponent);
		}
		
		for(Query query : problem.QUERIES) {
			boolean found = false;
			for(Set<Pos> component : connectedComponents) {
				if(component.contains(query.START)) {
					if(component.contains(query.END)) {
						if(problem.MAP[query.START.ROW][query.START.COL] == true) {
							answers.add("decimal");
						}
						else {
							answers.add("binary");
						}
						found = true;
						break;
					}
				}
			}
			if(!found) {
				answers.add("neither");
			}
		}
		return answers;
	}
	
	private static List<String> solveAllQueriesSimultaneously2(Problem problem) {
		List<String> answers = new ArrayList<>(problem.QUERIES.size());
		List<Set<Pos>> connectedComponents = new ArrayList<>();
		Set<Pos> independentPositions = new HashSet<>();
		for(int i = 0; i < problem.ROWS; i++) {
			for(int j = 0; j < problem.COLS; j++) {
				independentPositions.add(new Pos(i, j));
			}
		}
		while(!independentPositions.isEmpty()) {
			Set<Pos> newComponent = new HashSet<>();
			Pos pos = removeArbitraryElement(independentPositions);
			Set<Pos> frontier = new HashSet<>();
			frontier.add(pos);
			while(!frontier.isEmpty()) {
				Pos current = removeArbitraryElement(frontier);
				if(current.ROW > 0) {
					if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW - 1][current.COL]) {
						Pos north = new Pos(current.ROW - 1, current.COL);
						if(!newComponent.contains(north) && !frontier.contains(north)) {
							frontier.add(north);
							independentPositions.remove(north);
						}
					}
				}
				if(current.ROW < problem.ROWS - 1) {
					if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW + 1][current.COL]) {
						Pos south = new Pos(current.ROW + 1, current.COL);
						if(!newComponent.contains(south) && !frontier.contains(south)) {
							frontier.add(south);
							independentPositions.remove(south);
						}
					}
				}
				if(current.COL > 0) {
					if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW][current.COL - 1]) {
						Pos west = new Pos(current.ROW, current.COL - 1);
						if(!newComponent.contains(west) && !frontier.contains(west)) {
							frontier.add(west);
							independentPositions.remove(west);
						}
					}
				}
				if(current.COL < problem.COLS - 1) {
					if(problem.MAP[current.ROW][current.COL]== problem.MAP[current.ROW][current.COL + 1]) {
						Pos east = new Pos(current.ROW, current.COL + 1);
						if(!newComponent.contains(east) && !frontier.contains(east)) {
							frontier.add(east);
							independentPositions.remove(east);
						}
					}
				}
				newComponent.add(current);
			}
			
			connectedComponents.add(newComponent);
		}
		
		for(Query query : problem.QUERIES) {
			boolean found = false;
			for(Set<Pos> component : connectedComponents) {
				if(component.contains(query.START)) {
					if(component.contains(query.END)) {
						if(problem.MAP[query.START.ROW][query.START.COL] == true) {
							answers.add("decimal");
						}
						else {
							answers.add("binary");
						}
						found = true;
						break;
					}
				}
			}
			if(!found) {
				answers.add("neither");
			}
		}
		return answers;
	}

	
	private static Pos removeArbitraryElement(Set<Pos> set) {
		Iterator<Pos> iter = set.iterator();
		Pos element = iter.next();
		iter.remove();
		return element;
	}
	
	private static List<String> solveAllQueriesIndependently(Problem problem) {
		List<String> answers = new ArrayList<>(problem.QUERIES.size());
		for(Query query : problem.QUERIES) {
			answers.add(solveOneQuery(problem, query));
		}
		return answers;
	}
	
	private static String solveOneQuery(Problem problem, Query query) {
		if(problem.MAP[query.START.ROW][query.START.COL] != problem.MAP[query.END.ROW][query.END.COL]) {
			return "neither";
		}
		if(query.START.equals(query.END)) {
			if(problem.MAP[query.START.ROW][query.START.COL] == true) {
				return "decimal";
			}
			else {
				return "binary";
			}
		}
		PosComparator comp = new PosComparator(query.END);
		PriorityQueue<Pos> frontier = new PriorityQueue<>(comp);
		Set<Pos> visited = new HashSet<>();
		frontier.add(query.START);
		while(!frontier.isEmpty()) {
			Pos current = frontier.remove();
			if(current.equals(query.END)) {
				if(problem.MAP[current.ROW][current.COL] == true) {
					return "decimal";
				}
				else {
					return "binary";
				}
			}
			if(current.ROW > 0) {
				if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW - 1][current.COL]) {
					Pos north = new Pos(current.ROW - 1, current.COL);
					if(!visited.contains(north)) {
						frontier.add(north);
						visited.add(north);
					}
				}
			}
			if(current.ROW < problem.ROWS - 1) {
				if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW + 1][current.COL]) {
					Pos south = new Pos(current.ROW + 1, current.COL);
					if(!visited.contains(south)) {
						frontier.add(south);
						visited.add(south);
					}
				}
			}
			if(current.COL > 0) {
				if(problem.MAP[current.ROW][current.COL] == problem.MAP[current.ROW][current.COL - 1]) {
					Pos west = new Pos(current.ROW, current.COL - 1);
					if(!visited.contains(west)) {
						frontier.add(west);
						visited.add(west);
					}
				}
			}
			if(current.COL < problem.COLS - 1) {
				if(problem.MAP[current.ROW][current.COL]== problem.MAP[current.ROW][current.COL + 1]) {
					Pos east = new Pos(current.ROW, current.COL + 1);
					if(!visited.contains(east)) {
						frontier.add(east);
						visited.add(east);
					}
				}
			}
		}
		return "neither";
	}
	
	private static class Pos {
		public final int ROW;
		public final int COL;
		public Pos(int r, int c) {
			ROW = r;
			COL = c;
		}
		
		public static int manhattanDistance(Pos p1, Pos p2) {
			return Math.abs(p1.ROW - p2.ROW) + Math.abs(p1.COL - p2.COL);
		}
		
		@Override
		public boolean equals(Object obj) {
			if(obj == null) { return false; }
			if(obj.getClass() != this.getClass()) { return false; }
			Pos other = (Pos)obj;
			return (this.ROW == other.ROW && this.COL == other.COL);
		}
		
		@Override
		public int hashCode() {
			return ROW * 2641 + COL;
		}
		
		@Override
		public String toString() {
			return "(" + ROW + ", " + COL + ")";
		}
	}
	
	private static class PosComparator implements Comparator<Pos> {
		
		private Pos goal;
		public PosComparator(Pos g) {
			goal = g;
		}

		@Override
		public int compare(Pos arg0, Pos arg1) {
			int dist0 = Pos.manhattanDistance(arg0, goal);
			int dist1 = Pos.manhattanDistance(arg1, goal);
			if(dist0 != dist1) {
				return dist0 - dist1;
			}
			else if(arg0.ROW != arg1.ROW) {
				return arg0.ROW - arg1.ROW;
			}
			else {
				return arg0.COL - arg1.COL;
			}
		}
		
	}
	
	private static class Query {
		public final Pos START;
		public final Pos END;
		public Query(Scanner input) {
			START = new Pos(input.nextInt() - 1, input.nextInt() - 1);
			END = new Pos(input.nextInt() - 1, input.nextInt() - 1);
		}
	}
	
	private static class Problem {
		public final int ROWS;
		public final int COLS;
		public final boolean[][] MAP;
		public final List<Query> QUERIES;
		public Problem(Scanner input) {
			ROWS = input.nextInt();
			COLS = input.nextInt();
			MAP = new boolean[ROWS][COLS];
			for(int row = 0; row < ROWS; row++) {
				String line = input.next();
				for(int col = 0; col < COLS; col++) {
					MAP[row][col] = (line.charAt(col) == '1');
				}
			}
			int numQueries = input.nextInt();
			QUERIES = new ArrayList<>(numQueries);
			for(int i = 0; i < numQueries; i++) {
				QUERIES.add(new Query(input));
			}
		}
	}
	
}
