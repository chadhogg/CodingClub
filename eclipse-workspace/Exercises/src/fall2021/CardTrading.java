package fall2021;
import java.util.List;
import java.util.Random;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.PriorityQueue;

/**
 * A solution to https://open.kattis.com/problems/cardtrading
 * 
 * @author Chad Hogg
 */
public class CardTrading {

	/**
	 * A class that turns parses all the input into an easily usable object.
	 * 
	 * @author Chad Hogg
	 */
	private static class Problem {
		/** The number of cards that we start with. */
		public int deckSize;
		/** The number of different types of cards that exist. */
		public int cardTypes;
		/** The number of combos we want to end up with after trading. */
		public int desiredCombos;
		/** The quantity of each card that we have (card #1 at index 0, etc.). */
		public int[] deck;
		/** The costs to buy one copy of each card (card #1 at index 0, etc.). */
		public int[] buyCosts;
		/** The profit we get from selling one copy of each card (card #1 at index 0, etc.). */
		public int[] sellCosts;
		
		/**
		 * Parses a Problem from the input.
		 * 
		 * @param console A Scanner from which the input can be read.
		 */
		public Problem(Scanner console) {
			deckSize = console.nextInt();
			cardTypes = console.nextInt();
			desiredCombos = console.nextInt();
			deck = new int[cardTypes];
			for(int i = 0; i < deckSize; i++) {
				deck[console.nextInt() - 1]++;
			}
			buyCosts = new int[cardTypes];
			sellCosts = new int[cardTypes];
			for(int i = 0; i < cardTypes; i++) {
				buyCosts[i] = console.nextInt();
				sellCosts[i] = console.nextInt();
			}
		}
		
		public Problem(int maxDeckSize, int maxCardTypes, int maxDesiredCombos) {
			final int MIN_KT = 1;
			final int MAX_KT = 100000;
			final int MIN_COST = 1;
			final int MAX_COST = 1000000000;
			Random rand = new Random();
			if(maxCardTypes < MIN_KT || maxCardTypes > MAX_KT ||
					maxDesiredCombos < MIN_KT || maxDesiredCombos > MAX_KT ||
					maxDeckSize < MIN_KT || maxDeckSize > 2 * maxCardTypes) {
				throw new IllegalArgumentException("No bueno.");
			}
			cardTypes = rand.nextInt(maxCardTypes) + 1;
			desiredCombos = rand.nextInt(cardTypes) + 1;
			deckSize = rand.nextInt(Math.min(maxDesiredCombos, 2 * cardTypes)) + 1;
			deck = new int[cardTypes];
			buyCosts = new int[cardTypes];
			sellCosts = new int[cardTypes];
			assert(MIN_KT <= desiredCombos);
			assert(desiredCombos <= cardTypes);
			assert(cardTypes <= MAX_KT);
			assert(MIN_KT <= deckSize);
			assert(deckSize <= 2 * cardTypes);
			int totalCards = 0;
			List<Integer> growableIndices = new ArrayList<>();
			for(int i = 0; i < cardTypes; i++) {
				growableIndices.add(i);
			}
			while(totalCards < deckSize) {
				int indirectIndex = rand.nextInt(growableIndices.size());
				int directIndex = growableIndices.get(indirectIndex);
				deck[directIndex]++;
				if(deck[directIndex] == 2) {
					growableIndices.remove(indirectIndex);
				}
				totalCards++;
			}
			for(int i = 0; i < cardTypes; i++) {
				buyCosts[i] = rand.nextInt(MAX_COST) + MIN_COST;
				sellCosts[i] = rand.nextInt(MAX_COST) + MIN_COST;
			}
		}
		
		@Override
		public String toString() {
			StringBuffer buf = new StringBuffer();
			buf.append(deckSize).append(" ").append(cardTypes).append(" ").append(desiredCombos).append("\n");
			for(int i = 0; i < cardTypes; i++) {
				for(int j = 0; j < deck[i]; j++) {
					buf.append(i + 1).append(" ");
				}
			}
			buf.append("\n");
			for(int i = 0; i < cardTypes; i++) {
				buf.append(buyCosts[i]).append(" ").append(sellCosts[i]).append("\n");
			}
			return buf.toString();
		}
	}
	
	/**
	 * A card, with information about it.
	 * 
	 * @author Chad Hogg
	 */
	private static class Card implements Comparable<Card> {
		/** THe number on this card. */
		public int cardNum;
		/** What it would cost to have a combo of this card at the end. */
		public long costToMakePair;
		/** What I could earn by selling all my copies of this card. */
		public long valueToSellAll;
		
		/**
		 * Gets the opportunity cost of making a combo of this card vs. not doing so.
		 * 
		 * @return The sum of what it will cost to make a pair and what I will not be able to gain from selling my cards.
		 */
		public long getOpportunityCost() {
			return costToMakePair + valueToSellAll;
		}

		/**
		 * Compares two cards.
		 * Cards with a lower opportunity cost come first.  If tied on that, cards with lower numbers come first.
		 * 
		 * @param other The other card to compare to.
		 * @return A negative number if this card comes before the other; positive if opposite order; 0 if same.
		 */
		@Override
		public int compareTo(Card other) {
			long myOpportunityCost = valueToSellAll + costToMakePair;
			long hisOpportunityCost = other.valueToSellAll + other.costToMakePair;
			if(getOpportunityCost() != other.getOpportunityCost()) {
				if(getOpportunityCost() < other.getOpportunityCost()) {
					return -1;
				}
				else {
					return 1;
				}
			}
			else {
				return cardNum - other.cardNum;
			}
		}
		
		@Override
		public boolean equals(Object obj) {
			if(obj == null) { return false; }
			else if(obj.getClass() != this.getClass()) { return false; }
			else return (cardNum == ((Card)obj).cardNum);
		}
		
		@Override
		public int hashCode() {
			return cardNum;
		}
		
		@Override
		public String toString() {
			return Integer.toString(cardNum);
		}
	}
	
	/**
	 * A way to (attempt to) solve the problem.
	 * 
	 * @author Chad Hogg
	 */
	private static interface Solver {
		/**
		 * Finds the combos that make the greatest profit.
		 * 
		 * @param problem The statement of the problem.
		 * @param allCards A collection of all the cards.
		 * @return A set of those cards that should be combo-ed.
		 */
		public abstract Set<Card> chooseBestCombos(Problem problem, Set<Card> allCards);
	}
	
	/**
	 * An idea that made sense to me, and actually works but appeared not to because I wasn't using longs.
	 * 
	 * My intention was to calculate an "opportunity cost" of including a combo of each card in the final deck.
	 * This was calculated as the sum of what we have to pay to buy any needed cards of that type and what we miss out
	 *    on from not selling any cards of that type we have.
	 * It made sense to me that we should make combos of the k cards with the lowest opportunity cost.
	 * 
	 * @author Chad Hogg
	 */
	private static class MakeLowestOpportunityCostCombos implements Solver {
		@Override
		public Set<Card> chooseBestCombos(Problem problem, Set<Card> allCards) {
			PriorityQueue<Card> sorted = new PriorityQueue<>(allCards);
			Set<Card> chosen = new HashSet<>();
			while(chosen.size() < problem.desiredCombos) {
				chosen.add(sorted.remove());
			}
			return chosen;
		}
	}

	/**
	 * An idea that I think works, but at O(n*k) is apparently too slow for Kattis.
	 * 
	 * The idea here is that we don't really need to look at *every possible* combination, because there's really no
	 *    interactions between the different things that need to be chosen.
	 * Instead, we just keep track of the best k combos that are possible out of the m cards we've so far considered,
	 *    and when we consider a new card we either replace one of the existing k with it or not.
	 * This seems at least superficially similar to dynamic programming, though it is simple because the way my problems
	 *    overlap is straightforward.
	 * 
	 * @author Chad Hogg
	 */
	private static class DynamicProgrammingIdea implements Solver {
		@Override
		public Set<Card> chooseBestCombos(Problem problem, Set<Card> allCards) {
			Set<Card> chosenCards = new HashSet<>();
			for(Card cardToPossiblyAdd : allCards) {
				if(chosenCards.size() < problem.desiredCombos) {
					chosenCards.add(cardToPossiblyAdd);
				}
				else {
					Card bestToRemove = null;
					long bestProfitChange = 0;
					for(Card toReplace : chosenCards) {
						long change = getProfitChange(toReplace, cardToPossiblyAdd);
						//System.out.println("Profit change in replacing " + toReplace + " with " + cardToPossiblyAdd + " is " + change);
						if(change > bestProfitChange) {
							bestProfitChange = change;
							bestToRemove = toReplace;
						}
					}
					if(bestToRemove != null) {
						chosenCards.remove(bestToRemove);
						chosenCards.add(cardToPossiblyAdd);
					}
				}
			}
			//System.out.println("Choosing " + chosenCards);
			return chosenCards;			
		}
	}
	
	/**
	 * Recursive backtracking go brrrr.
	 * 
	 * Not a feasible solution, but a way to sanity-check the correctness of other ideas on small problems.
	 * 
	 * @author Chad Hogg
	 */
	public static class BruteForceIdea implements Solver {
		@Override
		public Set<Card> chooseBestCombos(Problem problem, Set<Card> allCards) {
			List<Set<Card>> partialSolutions = new LinkedList<>();
			partialSolutions.add(new HashSet<>());
			Set<Card> bestSolutionSoFar = null;
			long bestProfitSoFar = Long.MIN_VALUE;
			while(!partialSolutions.isEmpty()) {
				Set<Card> partialSolution = partialSolutions.remove(partialSolutions.size() - 1);
				for(Card card : allCards) {
					if(!partialSolution.contains(card)) {
						Set<Card> completerSolution = new HashSet<>(partialSolution);
						completerSolution.add(card);
						if(completerSolution.size() == problem.desiredCombos) {
							long profit = computeProfit(allCards, completerSolution);
							if(profit > bestProfitSoFar) {
								bestSolutionSoFar = completerSolution;
								bestProfitSoFar = profit;
							}
						}
						else {
							partialSolutions.add(completerSolution);
						}
					}
				}
			}
			return bestSolutionSoFar;
		}
	}
	
	/**
	 * Runs the program.
	 * 
	 * @param args Unused.
	 */
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		Problem problem = new Problem(console);
		Set<Card> cards = new HashSet<>();
		for(int i = 0; i < problem.cardTypes; i++) {
			Card card = new Card();
			card.cardNum = i + 1;
			card.costToMakePair = problem.buyCosts[i] * (2 - problem.deck[i]);
			card.valueToSellAll = problem.sellCosts[i] * problem.deck[i];
			cards.add(card);
		}
		Solver solver = new MakeLowestOpportunityCostCombos();
		Set<Card> chosenCards = solver.chooseBestCombos(problem, cards);
		System.out.println(computeProfit(cards, chosenCards));
	}

	/**
	 * Computes the total profit after wheeling-and-dealing.
	 * 
	 * @param allCards A set of all the cards that exist.
	 * @param chosenCards A set of the cards we have chosen to make combos of.
	 * @return  The total value earned by selling non-combo-ed cards minus the cost of all cards purchased to make combos.
	 */
	private static long computeProfit(Set<Card> allCards, Set<Card> chosenCards) {
		long profit = 0;
		for(Card card : allCards) {
			if(chosenCards.contains(card)) {
				profit -= card.costToMakePair;
			}
			else {
				profit += card.valueToSellAll;
			}
		}
		return profit;
	}

	/**
	 * Gets the net change in our profit from swapping out one combo for another.
	 * 
	 * @param removed The card we were combo-ing but will now be selling.
	 * @param added The card we were selling but are now combo-ing.
	 * @return The money we are saving by not buying any of the removed card, plus the money we are earning by selling
	 *    any of the removed cards we already had, minus the money we are spending to buy any of the added card, minus
	 *    they money we are no longer earning by selling any of the added card we have.
	 */
	private static long getProfitChange(Card removed, Card added) {
		return removed.costToMakePair + removed.valueToSellAll - added.valueToSellAll - added.costToMakePair;
	}
}
