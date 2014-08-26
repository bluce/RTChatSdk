package com.ztgame.embededvoice;

public class ByteUtil
{
	public static byte[] little_intToByte(int i, int len) {
		byte[] abyte = new byte[len];
		if (len == 1) {
			abyte[0] = (byte) (0xff & i);
		} else if (len == 2) {
			abyte[0] = (byte) (0xff & i);
			abyte[1] = (byte) ((0xff00 & i) >> 8);
		} else {
			abyte[0] = (byte) (0xff & i);
			abyte[1] = (byte) ((0xff00 & i) >> 8);
			abyte[2] = (byte) ((0xff0000 & i) >> 16);
			abyte[3] = (byte) ((0xff000000 & i) >> 24);
		}
		return abyte;
	}

	public static int little_bytesToInt(byte[] bytes) {
		int addr = 0;
		if (bytes.length == 1) {
			addr = bytes[0] & 0xFF;
		} else if (bytes.length == 2) {
			addr = bytes[0] & 0xFF;
			addr |= (((int) bytes[1] << 8) & 0xFF00);
		} else {
			addr = bytes[0] & 0xFF;
			addr |= (((int) bytes[1] << 8) & 0xFF00);
			addr |= (((int) bytes[2] << 16) & 0xFF0000);
			addr |= (((int) bytes[3] << 24) & 0xFF000000);
		}
		return addr;
	}

	/**
	 * int to byte[] 支持 1或者 4 个字节
	 * 
	 * @param i
	 * @param len
	 * @return
	 */
	public static byte[] big_intToByte(int i, int len) {
		byte[] abyte = new byte[len];
		;
		if (len == 1) {
			abyte[0] = (byte) (0xff & i);
		} else if (len == 2) {
			abyte[0] = (byte) ((i >>> 8) & 0xff);
			abyte[1] = (byte) (i & 0xff);
		} else {
			abyte[0] = (byte) ((i >>> 24) & 0xff);
			abyte[1] = (byte) ((i >>> 16) & 0xff);
			abyte[2] = (byte) ((i >>> 8) & 0xff);
			abyte[3] = (byte) (i & 0xff);
		}
		return abyte;
	}

	public static int big_bytesToInt(byte[] bytes) {
		int addr = 0;
		if (bytes.length == 1) {
			addr = bytes[0] & 0xFF;
		} else if (bytes.length == 2) {
			addr = bytes[0] & 0xFF;
			addr = (addr << 8) | (bytes[1] & 0xff);
		} else {
			addr = bytes[0] & 0xFF;
			addr = (addr << 8) | (bytes[1] & 0xff);
			addr = (addr << 8) | (bytes[2] & 0xff);
			addr = (addr << 8) | (bytes[3] & 0xff);
		}
		return addr;
	}
}
