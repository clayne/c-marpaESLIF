package org.parser.marpa;

import java.nio.ByteBuffer;

public class ESLIFRecognizer {
	private ESLIFGrammar             eslifGrammar                 = null;
	private ESLIFRecognizerInterface eslifRecognizerInterface     = null;
	private ByteBuffer               marpaESLIFRecognizerp        = null;
	private ByteBuffer               marpaESLIFRecognizerContextp = null;
	private boolean                  canContinue    = false;
	private boolean                  exhausted      = false;
	private native void              jniNew(ESLIFGrammar eslifGrammar);
	private native void              jniFree();
	private native void              jniScan(boolean initialEvents);
	private native void              jniResume();
	private native ESLIFEventType[]  jniEvent();

	public ESLIFRecognizer(ESLIFGrammar eslifGrammar, ESLIFRecognizerInterface eslifRecognizerInterface) {
		if (eslifGrammar == null) {
			throw new IllegalArgumentException("eslifGrammar must not be null");
		}
		if (eslifRecognizerInterface == null) {
			throw new IllegalArgumentException("eslifRecognizerInterface must not be null");
		}
		setEslifGrammar(eslifGrammar);
		setEslifRecognizerInterface(eslifRecognizerInterface);
		jniNew(eslifGrammar);
	}
	
	public void free() {
		jniFree();
	}

	public boolean isCanContinue() {
		return canContinue;
	}

	public boolean isExhausted() {
		return exhausted;
	}
	
	public void scan(boolean initialEvents) {
		jniScan(initialEvents);
	}

	public void resume() {
		jniResume();
	}
	/*
	 * ********************************************
	 * Private methods - used by the JNI
	 * ********************************************
	 */
	private ESLIFRecognizerInterface getEslifRecognizerInterface() {
		return eslifRecognizerInterface;
	}
	private void setEslifRecognizerInterface(ESLIFRecognizerInterface eslifRecognizerInterface) {
		this.eslifRecognizerInterface = eslifRecognizerInterface;
	}
	protected ESLIFLoggerInterface getLoggerInterface() {
		return (eslifGrammar != null) ? eslifGrammar.getLoggerInterface() : null;
	}
	private ESLIFGrammar getEslifGrammar() {
		return eslifGrammar;
	}
	private void setEslifGrammar(ESLIFGrammar eslifGrammar) {
		this.eslifGrammar = eslifGrammar;
	}
	private ByteBuffer getMarpaESLIFRecognizerp() {
		return marpaESLIFRecognizerp;
	}
	private void setMarpaESLIFRecognizerp(ByteBuffer marpaESLIFRecognizerp) {
		this.marpaESLIFRecognizerp = marpaESLIFRecognizerp;
	}
	private ByteBuffer getMarpaESLIFRecognizerContextp() {
		return marpaESLIFRecognizerContextp;
	}
	private void setMarpaESLIFRecognizerContextp(ByteBuffer marpaESLIFRecognizerContextp) {
		this.marpaESLIFRecognizerContextp = marpaESLIFRecognizerContextp;
	}
	private void setCanContinue(boolean canContinue) {
		this.canContinue = canContinue;
	}
	private void setExhausted(boolean exhausted) {
		this.exhausted = exhausted;
	}
}
