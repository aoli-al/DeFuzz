package al.aoli.defuzz.fuzzer.runners.jqf

import edu.berkeley.cs.jqf.fuzz.util.Counter

class DeFuzzCounter(private val index: Int): Counter(1) {

    override fun clear() {
        clearNative(index)
    }

    override fun getNonZeroSize(): Int {
        return getNonZeroSizeNative(index)
    }




    external fun clearNative(index: Int)
    external fun getNonZeroSizeNative(index: Int): Int
}