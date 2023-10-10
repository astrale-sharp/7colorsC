type Listener<T> = (arg: T) => void;
export type Event<T> = (listener: Listener<T>, options?: { autoDispose?: boolean }) => { dispose: () => boolean };

export class EventEmitter<T> {
  private listeners = new Set<Listener<T>>();

  register: Event<T> = (listener, options) => {
    const dispose = () => this.listeners.delete(listener);
    if (options?.autoDispose) {
      const origListener = listener;
      listener = (arg) => {
        dispose();
        origListener(arg);
      };
    }
    this.listeners.add(listener);
    return { dispose };
  };

  public fire(arg: T): void {
    for (const listener of this.listeners) listener(arg);
  }
}
