
// java example
typealias Observer = (event: String) -> Unit;

class EventSource {
    private var observers = mutableListOf<Observer>()

    private fun notifyObservers(event: String) {
        observers.forEach { it(event) }
    }

    fun addObserver(observer: Observer) {
        observers += observer
    }

    fun scanSystemIn() {
        val scanner = Scanner(System.`in`)
        while (scanner.hasNext()) {
            val line = scanner.nextLine()
            notifyObservers(line)
        }
    }
}

fun main(arg: List<String>) {
    println("Enter Text: ")
    val eventSource = EventSource()

    eventSource.addObserver { event ->
        println("Received response: $event")
    }

    eventSource.scanSystemIn()
}


// python example
class Observable:
    def __init__(self):
        self._observers = []

    def register_observer(self, observer):
        self._observers.append(observer)

    def notify_observers(self, *args, **kwargs):
        for obs in self._observers:
            obs.notify(self, *args, **kwargs)


class Observer:
    def __init__(self, observable):
        observable.register_observer(self)

    def notify(self, observable, *args, **kwargs):
        print("Got", args, kwargs, "From", observable)


subject = Observable()
observer = Observer(subject)
