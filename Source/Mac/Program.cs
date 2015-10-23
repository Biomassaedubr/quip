using MonoMac.AppKit;

namespace Quip {
  /// <summary>
  /// The application class.
  /// </summary>
  class Program {
    /// <summary>
    /// The application's entry point.
    /// </summary>
    /// <param name="arguments">The command-line arguments.</param>
    static void Main (string[] arguments) {
      NSApplication.Init();
      NSApplication.Main(arguments);
    }
  }
}