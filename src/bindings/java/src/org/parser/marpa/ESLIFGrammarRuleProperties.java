package org.parser.marpa;

/**
 * ESLIFGrammarRuleProperties is describing properties of a rule within a grammar.
 * 
 * Calls to grammar's currentRuleProperties() or rulePropertiesByLevel() methods outputs an instance of this class.
 *
 * <pre>
 * ESLIF eslif = new ESLIF(...)
 * ESLIFGrammar eslifGrammar = new ESLIFGrammar(...);
 * ESLIFGrammarProperties eslifGrammarProperties = eslifGrammar->currentRuleProperties(0);
 * ESLIFGrammarProperties eslifGrammarProperties = eslifGrammar->rulePropertiesByLevel(0, 0);
 * ...
 * eslifGrammar.free();
 * eslif.free()
 * </pre>
 */
public class ESLIFGrammarRuleProperties {
	  private int     id;
	  private String  description;
	  private String  show;
	  private int     lhsId;
	  private int     separatorId;
	  private int[]   rhsIds;
	  private int     exceptionId;
	  private String  action;
	  private String  discardEvent;
	  private boolean discardEventInitialState;
	  private int     rank;
	  private boolean nullRanksHigh;
	  private boolean sequence;
	  private boolean proper;
	  private int     minimum;
	  private boolean internal;
	  private int     propertyBitSet;
	  private boolean hideseparator;

	/*
	 * ********************************************
	 * Public methods
	 * ********************************************
	 */
	/**
	 * Creation of an ESLIFGrammarRuleProperties instance
	 * 
	 * @param id Rule id
	 * @param description Rule description
	 * @param show Rule show
	 * @param lhsId LHS id
	 * @param separatorId Separator Id
	 * @param rhsIds Array of RHS id
	 * @param exceptionId Exception id
	 * @param action Explicit action
	 * @param discardEvent Discard event name when this is a :discard rule
	 * @param discardEventInitialState Discard event initial state is on ?
	 * @param rank Rule rank
	 * @param nullRanksHigh Null ranks high ?
	 * @param sequence Is a sequence ?
	 * @param proper When it is a sequence, is separatation proper ?
	 * @param minimum When it is a sequence, mininum number of items
	 * @param internal Internal rule (happens only when loosen operator '||' is used) ?
	 * @param propertyBitSet Low-level property bit set
	 * @param hideseparator When it is a sequence, hide separator for action arguments ?
	 */
	public ESLIFGrammarRuleProperties(int id, String description, String show, int lhsId, int separatorId, int[] rhsIds, int exceptionId, String  action, String  discardEvent, boolean discardEventInitialState, int rank, boolean nullRanksHigh, boolean sequence, boolean proper, int minimum, boolean internal, int propertyBitSet, boolean hideseparator) {
		  this.id = id;
		  this.description = description;
		  this.show = show;
		  this.lhsId = lhsId;
		  this.separatorId = separatorId;
		  this.rhsIds = rhsIds;
		  this.exceptionId = exceptionId;
		  this.action = action;
		  this.discardEvent = discardEvent;
		  this.discardEventInitialState = discardEventInitialState;
		  this.rank = rank;
		  this.nullRanksHigh = nullRanksHigh;
		  this.sequence = sequence;
		  this.proper = proper;
		  this.minimum = minimum;
		  this.internal = internal;
		  this.propertyBitSet = propertyBitSet;
		  this.hideseparator = hideseparator;
	}

	/**
	 * @return Rule's id (always >= 0)
	 */
	public int getId() {
		return id;
	}

	/**
	 * @return Rule's description (auto-generated if there is not "name" keyword in the grammar)
	 */
	public String getDescription() {
		return description;
	}

	/**
	 * @return Rule's show
	 */
	public String getShow() {
		return show;
	}

	/**
	 * @return Rule's LHS symbol id (always >= 0)
	 */
	public int getLhsId() {
		return lhsId;
	}

	/**
	 * @return Rule's separator symbol id (< 0 if the rule is not a sequence)
	 */
	public int getSeparatorId() {
		return separatorId;
	}

	/**
	 * @return Rule's RHS ids (none for a null rule)
	 */
	public int[] getRhsIds() {
		return rhsIds;
	}

	/**
	 * @return Rule's exception id (< 0 if there is no exception)
	 */
	public int getExceptionId() {
		return exceptionId;
	}

	/**
	 * @return Rule's action (null if none)
	 */
	public String getAction() {
		return action;
	}

	/**
	 * @return Rule's discard event name (only when LHS is ":discard" and "event" keyword is present)
	 */
	public String getDiscardEvent() {
		return discardEvent;
	}

	/**
	 * @return Rule's discard initial state is on ?
	 */
	public boolean isDiscardEventInitialState() {
		return discardEventInitialState;
	}

	/**
	 * Alias to isDiscardEventInitialState()
	 * 
	 * @return Rule's discard initial state is on ?
	 */
	public boolean getDiscardEventInitialState() {
		return isDiscardEventInitialState();
	}

	/**
	 * @return Rule's rank (defaults to 0)
	 */
	public int getRank() {
		return rank;
	}

	/**
	 * @return Rule rank high when it is a nullable ?
	 */
	public boolean isNullRanksHigh() {
		return nullRanksHigh;
	}

	/**
	 * Alias to isNullRanksHigh()
	 * 
	 * @return Rule rank high when it is a nullable ?
	 */
	public boolean getNullRanksHigh() {
		return isNullRanksHigh();
	}

	/**
	 * @return Rule is a sequence ?
	 */
	public boolean isSequence() {
		return sequence;
	}

	/**
	 * Alias to isSequence()
	 * 
	 * @return Rule is a sequence ?
	 */
	public boolean getSequence() {
		return isSequence();
	}

	/**
	 * @return Rule's separation is proper ? (meaningful only when it is sequence)
	 */
	public boolean isProper() {
		return proper;
	}

	/**
	 * Alias to isProper()
	 * 
	 * @return Rule's separation is proper ? (meaningful only when it is sequence)
	 */
	public boolean getProper() {
		return isProper();
	}

	/**
	 * @return Rule's minimum number of RHS (meaningful only when rule is a sequence)
	 */
	public int getMinimum() {
		return minimum;
	}

	/**
	 * @return Rule is internal ? (possible only when there is the loosen operator "||")
	 */
	public boolean isInternal() {
		return internal;
	}

	/**
	 * Alias to isInternal()
	 * 
	 * @return Rule is internal ? (possible only when there is the loosen operator "||")
	 */
	public boolean getInternal() {
		return isInternal();
	}

	/**
	 * @return Rule's low-level property bits
	 */
	public int getPropertyBitSet() {
		return propertyBitSet;
	}

	/**
	 * @return Hide separator in action callback ? (meaningful only when rule is a sequence)
	 */
	public boolean isHideseparator() {
		return hideseparator;
	}

	/**
	 * Alias to isHideseparator()
	 * 
	 * @return Hide separator in action callback ? (meaningful only when rule is a sequence)
	 */
	public boolean getHideseparator() {
		return isHideseparator();
	}
}
