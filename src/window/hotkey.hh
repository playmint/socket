#ifndef SSC_WINDOW_HOTKEY_H
#define SSC_WINDOW_HOTKEY_H

#include "../core/platform.hh"
#include "../core/types.hh"
#include "../ipc/ipc.hh"

namespace SSC {
  // forward
  class Window;

  class HotKeyCodeMap {
    public:
      using Code = unsigned int;
      using Key = String;
      using Map = std::map<Key, Code>;

      Map keys;
      Map modifiers;

      HotKeyCodeMap ();
      const Code get (Key key) const;
      const bool isModifier (Key key) const;
      const bool isKey (Key key) const;
  };

  class HotKeyBinding {
    public:
      using ID = uint32_t;
      using Hash = std::size_t;
      using Token = String;
      using Sequence = Vector<Token>;
      using Expression = String;

      static Sequence parseExpression (Expression input);

      HotKeyCodeMap codeMap;
      HotKeyCodeMap::Code modifiers = 0;
      HotKeyCodeMap::Code keys = 0;
      Hash hash = 0;
      ID id = 0;

      Expression expression;
      Sequence sequence;

    #if defined(__APPLE__) && (!TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR)
      // Apple Carbon API
      EventHotKeyRef eventHotKeyRef;
    #elif defined(__linux__) && !defined(__ANDROID__)
      // TODO
    #elif defined(_WIN32)
      // TODO
    #endif

      HotKeyBinding (ID id, Expression input);
      bool isValid () const;
  };

  class HotKeyContext {
    public:
      using Bindings = std::map<HotKeyBinding::ID, HotKeyBinding>;

    #if defined(__APPLE__) && (!TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR)
      // Apple Carbon API
      EventTargetRef eventTarget;
    #elif defined(__linux__) && !defined(__ANDROID__)
      // TODO
    #elif defined(_WIN32)
      // TODO
    #endif

      Window* window = nullptr;
      IPC::Bridge* bridge = nullptr;

      // state
      Mutex mutex;
      // owned by the context
      Vector<HotKeyBinding::ID> bindingIds;
      Bindings& bindings;

      HotKeyContext (Window* window);
      ~HotKeyContext ();

      void init (IPC::Bridge* bridge);
      void reset ();
      const HotKeyBinding bind (HotKeyBinding::Expression expression);
      bool unbind (HotKeyBinding::ID id);
      bool hasBindingForExpression (HotKeyBinding::Expression expression) const;
      const HotKeyBinding getBindingForExpression (HotKeyBinding::Expression expression) const;
      bool onHotKeyBindingCallback (HotKeyBinding::ID);
  };
}

#endif
