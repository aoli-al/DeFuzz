package al.aoli.defuzz.fuzzer.runners.jqf

import edu.berkeley.cs.jqf.fuzz.util.Counter
import edu.berkeley.cs.jqf.fuzz.util.ICoverage
import org.eclipse.collections.api.list.primitive.IntList
import org.eclipse.collections.impl.list.mutable.primitive.IntArrayList

class DeFuzzCoverage: ICoverage<DeFuzzCounter> {
    val counter = DeFuzzCounter()
    override fun size(): Int {
        return (1 shl 8)
    }

    override fun getNonZeroCount(): Int {
        return counter.nonZeroSize
    }

    override fun getCovered(): IntList {
        return IntArrayList()
    }

    override fun computeNewCoverage(p0: ICoverage<*>?): IntList {
        return IntArrayList()
    }

    override fun clear() {
    }

    override fun updateBits(that: ICoverage<*>?): Boolean {
        if (that !is DeFuzzCoverage) {
            return false
        }
        synchronized(this.counter) {
            synchronized(that.counter) {
            }
        }
        return false
    }

    override fun nonZeroHashCode(): Int {
        return 0
    }

    override fun getCounter(): Counter {
        return counter
    }

    override fun copy(): ICoverage<DeFuzzCounter> {
        return this
    }

    companion object {
        var currentIndex = 0
    }
}