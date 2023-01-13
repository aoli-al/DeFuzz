package al.aoli.defuzz.fuzzer.runners.jqf

import edu.berkeley.cs.jqf.fuzz.util.Counter
import edu.berkeley.cs.jqf.fuzz.util.ICoverage
import org.eclipse.collections.api.list.primitive.IntList

class DeFuzzCoverage: ICoverage<DeFuzzCounter> {
    val counter = DeFuzzCounter(currentIndex++)
    override fun size(): Int {
        return (1 shl 8)
    }

    override fun getNonZeroCount(): Int {
        counter.nonZeroSize
    }

    override fun getCovered(): IntList {
    }

    override fun computeNewCoverage(p0: ICoverage<*>?): IntList {
    }

    override fun clear() {
    }

    override fun updateBits(p0: ICoverage<*>?): Boolean {
    }

    override fun nonZeroHashCode(): Int {
    }

    override fun getCounter(): Counter {
    }

    override fun copy(): ICoverage<DeFuzzCounter> {
    }

    companion object {
        var currentIndex = 0
    }
}