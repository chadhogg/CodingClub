package justforfun;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;

/*
 * Some profiling results on my attempt to build a hard problem:
 * 
 * solve1 used 2084.1 seconds
 *   100% in 50,000 invocations of Collections.min()
 * 
 * solve2 used 39.0 seconds
 *   62% in 569,000 invocations of CatDB.updateCat()
 *     29% in 22,295,000 invocations of HashMap.remove()
 *     22% in 22,865,000 invocations of HashMap.get()
 *     11% in itself
 *   28% in 181,000 invocations of CatDB.removeCat()
 *     13% in 10,811,000 invocations of HashMap.remove()
 *     10% in 10,811,000 invocations of ArrayList.get()
 *      5% in itself
 *    5% in 50,000 invocations of PrintStream.println()
 * 
 * solve4 used 6.5 seconds
 *   29% in 207,000 invocations of AbstractQueue.remove()
 *   21% in 819,000 invocations of AbstractQueue.add()
 *   18% in 50,000 invocations of PrintStream.println()
 *   11% in 619,000 invocations of HashMap.get()
 *    8% in 769,000 invocations of HashMap.put()
 *    5% in itself
 *    
 * Without the profiler:
 * solve1 takes 3.97 seconds
 * solve2 takes 1.52 seconds
 * solve4 takes 1.25 seconds
 */

/**
 * A solution to https://open.kattis.com/problems/doctorkattis.
 * 
 * It is too slow, so I implemented the solve4 algorithm in C++ and had that solution accepted.
 * 
 * @author Chad Hogg
 */
public class DoctorKattis {

	/**
	 * A type of command.
	 * 
	 * It feels like there should be some abstract methods here, but what each command does is totally different.
	 * 
	 * @author Chad Hogg
	 */
	private static class Command {
		
		/**
		 * Extracts the relevant data for one command from the input, creates, and returns that command.
		 * 
		 * @param input A Scanner from which data about a command can be read.
		 * @return The command.
		 */
		private static Command fromScanner(Scanner input) {
			int type = input.nextInt();
			switch(type) {
			case 0:
				return new ArriveAtClinic(input.next(), input.nextInt());
			case 1:
				return new UpdateInfectionLevel(input.next(), input.nextInt());
			case 2:
				return new Treated(input.next());
			case 3:
				return new Query();
			default:
				throw new IllegalStateException("Invalid input.");
			}
		}
	}
	
	/**
	 * A command informing us that a new cat has arrived at the clinic.
	 * 
	 * @author Chad Hogg
	 */
	private static class ArriveAtClinic extends Command {
		/** The name of the arriving cat. */
		public final String CATNAME;
		/** The initial infection level of the arriving cat. */
		public final int INFECTION_LEVEL;
		
		/**
		 * Constructs a new arrival.
		 * 
		 * @param c The name of the arriving cat.
		 * @param i The initial infection level of the arriving cat.
		 */
		public ArriveAtClinic(String c, int i) {
			CATNAME = c;
			INFECTION_LEVEL = i;
		}
	}
	
	/**
	 * A command informing us that a cat's infection level has increased.
	 * 
	 * @author Chad Hogg
	 */
	private static class UpdateInfectionLevel extends Command {
		/** The name of the cat that is getting sicker. */
		public final String CATNAME;
		/** HOw much sicker the cat got. */
		public final int INCREASE_INFECTION;
		
		/**
		 * Constructs a new update.
		 * 
		 * @param c The name of the updated cat.
		 * @param i The change in infection level for the updated cat.
		 */
		public UpdateInfectionLevel(String c, int i) {
			CATNAME = c;
			INCREASE_INFECTION = i;
		}
	}
	
	/**
	 * A command informing us that a cat has been treated and discharged.
	 * 
	 * @author Chad Hogg
	 */
	private static class Treated extends Command {
		/** The name of the treated cat. */
		public final String CATNAME;
		
		/**
		 * Constructs a new treated.
		 * 
		 * @param c The name of the cat who was treated.
		 */
		public Treated(String c) {
			CATNAME = c;
		}
	}
	
	/**
	 * A command requesting the name of the highest-priority cat.
	 * 
	 * @author Chad Hogg
	 */
	private static class Query extends Command {
	}
	
	/**
	 * Information about a cat at the clinic.
	 * 
	 * @author Chad Hogg
	 */
	private static class Cat implements Comparable<Cat> {
		/** The name of the cat. */
		public final String NAME;
		/** The cat's current infection level. */
		public int level;
		/** When the cat arrived at the clinic. */		
		public final int ARRIVE_TIME;
		
		/**
		 * Constructs a new cat.
		 * 
		 * @param n The name of the new cat.
		 * @param l The infection level of the new cat.
		 * @param a When the new cat arrived at the clinic.
		 */
		public Cat(String n, int l, int a) {
			NAME = n;
			level = l;
			ARRIVE_TIME = a;
		}
		
		@Override
		public boolean equals(Object obj) {
			if(obj == null) { return false; }
			if(this.getClass() != obj.getClass()) { return false; }
			Cat other = (Cat)obj;
			return NAME.equals(other.NAME) && level == other.level && ARRIVE_TIME == other.ARRIVE_TIME;
		}
		
		@Override
		public int hashCode() {
			return NAME.hashCode();
		}
		
		@Override
		public String toString() {
			return NAME + "(" + level + " " + ARRIVE_TIME + ")";
		}
		
		@Override
		public int compareTo(Cat other) {
			// Higher infection level should come first.
			int levelResult = (other.level - this.level);
			if(levelResult != 0) {
				return levelResult;
			}
			// Earlier arrival time should come first.
			return this.ARRIVE_TIME - other.ARRIVE_TIME;
		}
	}
	
	/**
	 * A class that generates problems to solve.
	 * 
	 * @author Chad Hogg
	 */
	@SuppressWarnings("unused")
	private static class ProblemGenerator {
		
		/**
		 * Generates the N-th name that contains only capital letters.
		 * 
		 * @param num THe N.
		 * @return A name from the sequence A, B, ... Z, AA, AB, ... ZZ, AAA, ...
		 */
		private static String genName(int num) {
			if(num < 26) {
				return "" + (char)('A' + num);
			}
			else {
				return genName((num / 26) - 1) + (char)('A' + (num % 26));
			}
		}
		
		/**
		 * Generates a problem.
		 * 
		 * @param numCommands The number of commands in the problem.
		 * @param percentNew The likelihood that each command should be an arrival.
		 * @param percentUpdate The likelihood that each command should be an update.
		 * @param percentTreat The likelihood that each command should be a treated.
		 * @param percentQuery The likelihood that each command should be a query.
		 * @return A string containing the problem.
		 */
		public static String generateProblem(int numCommands, double percentNew, double percentUpdate, double percentTreat, double percentQuery) {
			final int MAX_COMMANDS = 1000000;
			final int MAX_CATS = 200000;
			final int MAX_INFECTION = 100;
			final int MIN_INFECTION = 30;
			final int MAX_GROWTH = 70;
			final double PERCENT_TREAT_HIGHEST_PRIORITY = 0.75;
			final double PERCENT_SMALL_CHANGE = 0.75;
			if(numCommands < 1 || numCommands > MAX_COMMANDS) {
				throw new IllegalArgumentException("Bad number of commands.");
			}
			if(Math.abs(percentNew + percentUpdate + percentTreat + percentQuery - 1.0) > 0.00001) {
				throw new IllegalArgumentException("Bad percentage total.");
			}
			if(percentNew <= 0.0 || percentUpdate <= 0.0 || percentTreat <= 0.0 || percentQuery <= 0.0) {
				throw new IllegalArgumentException("Percentages must be positive.");
			}
			if(percentNew * numCommands > MAX_CATS) {
				throw new IllegalArgumentException("Proposed problem would create too many cats.");
			}
			StringBuilder sb = new StringBuilder();
			sb.append(numCommands).append("\n");
			Map<String, Integer> cats = new HashMap<>();
			List<String> namesInUse = new ArrayList<>(200000);
			List<String> namesEverUsed = new ArrayList<>(200000);
			Random rand = new Random(0);
			for(int i = 0; i < numCommands; i++) {
				boolean didCommand = false;
				double chance = rand.nextDouble();
				if(chance < percentUpdate && !namesInUse.isEmpty()) {
					String name = namesInUse.get(rand.nextInt(namesInUse.size()));
					if(cats.get(name) < MAX_INFECTION) {
						int amount;
						if(rand.nextDouble() < PERCENT_SMALL_CHANGE) {
							amount = rand.nextInt(Math.min(10, MAX_INFECTION - cats.get(name)) + 1);
						}
						else {
							amount = rand.nextInt(Math.min(MAX_GROWTH, MAX_INFECTION - cats.get(name)) + 1);
						}
						cats.put(name, cats.get(name) + amount);
						sb.append("1 ").append(name).append(" ").append(amount).append("\n");
						didCommand = true;
					}
				}
				if(chance < percentUpdate + percentNew && namesEverUsed.size() < MAX_CATS && !didCommand) {
					String newName = genName(namesEverUsed.size());
					namesEverUsed.add(newName);
					namesInUse.add(newName);
					int infection = rand.nextInt(MAX_INFECTION - MIN_INFECTION + 1) + MIN_INFECTION;
					cats.put(newName, infection);
					sb.append("0 ").append(newName).append(" ").append(infection).append("\n");
					didCommand = true;
				}
				if(chance < percentUpdate + percentNew + percentTreat && !namesInUse.isEmpty() && !didCommand) {
					String chosenName;
					if(rand.nextDouble() < PERCENT_TREAT_HIGHEST_PRIORITY) {
						chosenName = null;
						int highestInfect = -1;
						for(String name : namesInUse) {
							int infect = cats.get(name);
							if(infect > highestInfect) {
								highestInfect = infect;
								chosenName = name;
								if(highestInfect == MAX_INFECTION) {
									break;
								}
							}
						}
						
					}
					else {
						chosenName = namesInUse.get(rand.nextInt(namesInUse.size()));
					}
					namesInUse.remove(chosenName);
					cats.remove(chosenName);
					sb.append("2 ").append(chosenName).append("\n");
					didCommand = true;
				}
				if(!didCommand) {
					sb.append("3\n");
				}
			}
			return sb.toString();
		}
	}
	
	/**
	 * A custom data structure designed to make all operations efficient.
	 * 
	 * @author Chad Hogg
	 */
	private static class CatDB {
		/** The lowest possible infection level. */
		public final static int MIN_LEVEL = 30;
		/** The highest possible infection level. */
		public final static int MAX_LEVEL = 100;
		/** A list of Name->Cat maps, one per infection level. */
		private List<Map<String, Cat>> maps;
		
		/**
		 * Constructs a new, empty cat database.
		 */
		public CatDB() {
			maps = new ArrayList<>(MAX_LEVEL + 1);
			for(int index = 0; index < MIN_LEVEL; index++) {
				maps.add(null);
			}
			for(int index = MIN_LEVEL; index <= MAX_LEVEL; index++) {
				maps.add(new HashMap<>());
			}
		}
		
		/**
		 * Adds a new cat to the database.
		 * 
		 * @param cat The cat to add.
		 */
		public void addCat(Cat cat) {
			maps.get(cat.level).put(cat.NAME, cat);
		}
		
		/**
		 * Updates the infection level for a cat.
		 * 
		 * @param name The name of the cat this is more severely infected.
		 * @param levelUp How much the infection has increased.
		 */
		public void updateCat(String name, int levelUp) {
			Cat cat = null;
			for(int index = MIN_LEVEL; index <= MAX_LEVEL; index++) {
				cat = maps.get(index).remove(name);
				if(cat != null) {
					break;
				}
			}
			cat.level += levelUp;
			maps.get(cat.level).put(cat.NAME, cat);
		}
		
		/**
		 * Removes a cat from the database.
		 * 
		 * @param name The name of the cat who has been discharged.
		 */
		public void removeCat(String name) {
			Cat cat = null;
			for(int index = MIN_LEVEL; index <= MAX_LEVEL; index++) {
				cat = maps.get(index).remove(name);
				if(cat != null) {
					break;
				}
			}
		}
		
		/**
		 * Gets the name of the highest-priority cat.
		 * 
		 * @return The name of the highest-priority cat.
		 */
		public String getHighestPriorityCat() {
			Cat cat = null;
			for(int index = MAX_LEVEL; index >= MIN_LEVEL; index--) {
				if(!maps.get(index).isEmpty()) {
					cat = Collections.min(maps.get(index).values());
					return cat.NAME;
				}
			}
			return "The clinic is empty";
		}
	}
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		List<Command> commands = readInput(new Scanner(System.in));
		//long before = System.nanoTime();
		solve4(commands);
		//long after = System.nanoTime();
		//System.out.println("Took " + (after - before) / 1000000000.0 + "s");
	}
	
	/**
	 * Reads the user input.
	 * 
	 * @return A list of commands to be executed.
	 */
	private static List<Command> readInput(Scanner input) {
		int numCommands = input.nextInt();
		List<Command> commands = new ArrayList<>(numCommands);
		for(int i = 0; i < numCommands; i++) {
			commands.add(Command.fromScanner(input));
		}
		return commands;
	}
	
	/**
	 * Answers all query commands, in order.
	 * 
	 * Note that this strategy is too slow.
	 * I don't have a profiler available, but I'm assuming that is because of the O(200,000) search for the highest priority cat
	 *   that happens O(1,000,000) times.
	 * 
	 * @param commands A list of commands to be executed.
	 */
	@SuppressWarnings("unused")
	private static void solve1(List<Command> commands) {
		Map<String, Cat> cats = new HashMap<>();
		for(int i = 0; i < commands.size(); i++) {
			Command cmd = commands.get(i);
			if(cmd instanceof ArriveAtClinic) {
				ArriveAtClinic aac = (ArriveAtClinic)cmd;
				cats.put(aac.CATNAME, new Cat(aac.CATNAME, aac.INFECTION_LEVEL, i));
			}
			else if(cmd instanceof UpdateInfectionLevel) {
				UpdateInfectionLevel uil = (UpdateInfectionLevel)cmd;
				cats.get(uil.CATNAME).level += uil.INCREASE_INFECTION;
			}
			else if(cmd instanceof Treated) {
				Treated t = (Treated)cmd;
				cats.remove(t.CATNAME);
			}
			else if(cmd instanceof Query) {
				if(cats.isEmpty()) {
					System.out.println("The clinic is empty");
				}
				else {
					Cat worst = Collections.min(cats.values());
					System.out.println(worst.NAME);
				}
			}
			else {
				throw new IllegalStateException("How did I get an unknown command type?");
			}
		}
	}
	
	/**
	 * A second attempt at solving the problem.
	 * 
	 * This uses a custom data structure that separates cats into different buckets by their infection level.
	 * This requires removing and re-inserting when a cat's infection level changes.
	 * But if infection levels are uniformly distributed, it will reduce many costs by a factor of 70.
	 * 
	 * This is still too slow.
	 * 
	 * @param commands The commands to run.
	 */
	@SuppressWarnings("unused")
	private static void solve2(List<Command> commands) {
		CatDB cats = new CatDB();
		for(int i = 0; i < commands.size(); i++) {
			Command cmd = commands.get(i);
			if(cmd instanceof ArriveAtClinic) {
				ArriveAtClinic aac = (ArriveAtClinic)cmd;
				cats.addCat(new Cat(aac.CATNAME, aac.INFECTION_LEVEL, i));
			}
			else if(cmd instanceof UpdateInfectionLevel) {
				UpdateInfectionLevel uil = (UpdateInfectionLevel)cmd;
				cats.updateCat(uil.CATNAME, uil.INCREASE_INFECTION);
			}
			else if(cmd instanceof Treated) {
				Treated t = (Treated)cmd;
				cats.removeCat(t.CATNAME);
			}
			else if(cmd instanceof Query) {
				System.out.println(cats.getHighestPriorityCat());
			}
			else {
				throw new IllegalStateException("How did I get an unknown command type?");
			}
		}		
	}
	
	/**
	 * A new idea based on caching updates instead of doing them immediately.
	 * The goal is to never need to do a linear-time lookup in any data structure.
	 * We will store information about the cats in the clinic in a priority queue, ordered by infection level + arrival time.
	 * When an update comes in, we will store the information about the update in a separate data structure that allows O(1) insertions and searches by name.
	 * When a discharge comes in we will store the information about the discharge in a separate data structure that allows O(1) insertions and searches by name.
	 * When a query comes in we will remove from the priority queue until we find one that didn't get updated or discharged.
	 *   If it was discharged, we simple move on.
	 *   If it was updated, we apply the update and re-insert, since it might no longer be highest priority.
	 * I just realized when writing this that it doesn't work -- because the updates get us closer to being highest priority instead of
	 *    further away, I can't assume that the highest priority cat without an update is the one to choose.
	 * 
	 * As predicted, when I run it the queries sometimes return the wrong cat.
	 * 
	 * @param commands The list of commands.
	 */
	@SuppressWarnings("unused")
	private static void solve3(List<Command> commands) {
		PriorityQueue<Cat> cats = new PriorityQueue<>();
		Map<String, Integer> updates = new HashMap<>();
		Set<String> discharged = new HashSet<>();
		for(int i = 0; i < commands.size(); i++) {
			Command cmd = commands.get(i);
			if(cmd instanceof ArriveAtClinic) {
				ArriveAtClinic aac = (ArriveAtClinic)cmd;
				cats.add(new Cat(aac.CATNAME, aac.INFECTION_LEVEL, i));
			}
			else if(cmd instanceof UpdateInfectionLevel) {
				UpdateInfectionLevel uil = (UpdateInfectionLevel)cmd;
				if(updates.containsKey(uil.CATNAME)) {
					updates.put(uil.CATNAME, updates.get(uil.CATNAME) + uil.INCREASE_INFECTION);
				}
				else {
					updates.put(uil.CATNAME, uil.INCREASE_INFECTION);
				}
			}
			else if(cmd instanceof Treated) {
				Treated t = (Treated)cmd;
				discharged.add(t.CATNAME);
			}
			else if(cmd instanceof Query) {
				boolean finished = false;
				while(!finished && !cats.isEmpty()) {
					Cat highestPriority = cats.remove();
					if(discharged.contains(highestPriority.NAME)) {
						discharged.remove(highestPriority.NAME);
					}
					else if(updates.containsKey(highestPriority.NAME)) {
						highestPriority.level += updates.get(highestPriority.NAME);
						updates.remove(highestPriority.NAME);
						cats.add(highestPriority);
					}
					else {
						System.out.println(highestPriority.NAME);
						cats.add(highestPriority);
						finished = true;
					}
				}
				if(cats.isEmpty()) {
					System.out.println("The clinic is empty");
				}					
			}
			else {
				throw new IllegalStateException("What is this command?");
			}
		}
	}
	
	/**
	 * Yet another attempt.
	 * 
	 * This time, when a cat gets updated I will add the updated version to the heap *without removing the old one*,
	 *   since finding it to remove it is expensive.
	 * This is going to make my priority queue grow much larger, but since all operations I will do on it are O(log n), we won't worry about it.
	 * I will also track what the most recent version of a cat looks like, so that when I pull one off of the heap
	 *   if it is not the most recent version I just discard it.
	 * Adding a new cat is O(log c), discharging a cat is O(1), updating a cat is O(log c), and getting the highest priority cat is
	 *    O(c log c), where c is the number of non-query commands processes so far.
	 * But each non-query command contributes one O(log c) operation to one of the query commands and nothing to the others, so some
	 *    kind of amortized analysis could be done.
	 *   
	 * @param commands The list of commands to run.
	 */
	private static void solve4(List<Command> commands) {
		PriorityQueue<Cat> cats = new PriorityQueue<>();
		Map<String, Cat> current = new HashMap<>();
		for(int i = 0; i < commands.size(); i++) {
			Command cmd = commands.get(i);
			if(cmd instanceof ArriveAtClinic) {
				ArriveAtClinic aac = (ArriveAtClinic)cmd;
				Cat newCat = new Cat(aac.CATNAME, aac.INFECTION_LEVEL, i);
				cats.add(newCat);
				current.put(aac.CATNAME, newCat);
			}
			else if(cmd instanceof UpdateInfectionLevel) {
				UpdateInfectionLevel uil = (UpdateInfectionLevel)cmd;
				Cat cat = current.get(uil.CATNAME);
				Cat revisedCat = new Cat(cat.NAME, cat.level + uil.INCREASE_INFECTION, cat.ARRIVE_TIME);
				current.put(uil.CATNAME, revisedCat);
				cats.add(revisedCat);
			}
			else if(cmd instanceof Treated) {
				Treated t = (Treated)cmd;
				current.remove(t.CATNAME);
			}
			else if(cmd instanceof Query) {
				boolean finished = false;
				while(!finished && !cats.isEmpty()) {
					//System.out.println("Queue: " + cats);
					//System.out.println("Current: " + current);
					Cat highestPriority = cats.remove();
					if(current.containsKey(highestPriority.NAME) && current.get(highestPriority.NAME).equals(highestPriority)) {
						System.out.println(highestPriority.NAME);
						cats.add(highestPriority);
						finished = true;
					}
				}
				if(cats.isEmpty()) {
					System.out.println("The clinic is empty");
				}					
			}
			else {
				throw new IllegalStateException("What is this command?");
			}
		}
	}
}
