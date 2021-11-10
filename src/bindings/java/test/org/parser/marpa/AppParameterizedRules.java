package org.parser.marpa;

import java.io.BufferedReader;
import java.io.StringReader;
import java.util.concurrent.TimeUnit;

/**
 * Import/export Application
 */
public class AppParameterizedRules implements Runnable {
	ESLIFLoggerInterface eslifLogger;
	
	/**
	 * @param eslifLogger logger interface
	 */
	public AppParameterizedRules(ESLIFLoggerInterface eslifLogger) {
		this.eslifLogger = eslifLogger;
	}

	public void run() {
		ESLIF eslif;
		try {
			eslif = ESLIF.getInstance(eslifLogger);
		} catch (InterruptedException e) {
			e.printStackTrace();
			return;
		}

		if (false) {
			eslifLogger.info(" ATTACH ME");
			try {
				TimeUnit.SECONDS.sleep(10);
			} catch (InterruptedException e) {
				System.err.println(e);
			}
		}

		final String grammar = 
				"/*\n" +
				" * Example of parameterized rules\n" +
				"*/\n" +
				":discard ::= /[\\s]+/\n" +
				"\n" +
				"top  ::= rhs1\n" +
				"rhs1 ::= . => rhs->(1, nil, 'Input should be \"1\"')\n" +
				"       | . => rhs->(2, nil, 'Input should be \"2\"')\n" +
				"       | . => rhs->(3, nil, 'Input should be \"3\"')\n" +
				"       | . => rhs->(4, nil, 'Input should be \"4\"')\n" +
				"\n";

		String[] strings = {
						"5"
						};  

		ESLIFGrammar eslifGrammar = null;
		try {
			eslifGrammar = new ESLIFGrammar(eslif, grammar);
			eslifLogger.info("G1: " + eslifGrammar.showByLevel(0));
			for (int i = 0; i < strings.length; i++) {
				String string = new String(strings[i]);
	
				BufferedReader reader = new BufferedReader(new StringReader(string));
				AppRecognizer eslifAppRecognizer = new AppRecognizer(reader, eslifLogger);
				AppValue eslifAppValue = new AppValue();
				eslifLogger.info("Testing parse() on " + string);
				try {
					if (false) {
						eslifLogger.info(" ATTACH ME - SLEEPING 10 SECONDS");
						try {
							TimeUnit.SECONDS.sleep(10);
						} catch (InterruptedException e) {
							System.err.println(e);
						}
					}
					if (eslifGrammar.parse(eslifAppRecognizer, eslifAppValue)) {
						eslifLogger.info("Result: " + eslifAppValue.getResult());
					}
				} catch (Exception e) {
					System.err.println("Failed to parse " + string + ": " + e.getMessage());
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			return;
		} finally {
			if (eslifGrammar != null) {
				try {
					eslifGrammar.free();
				} catch (ESLIFException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
