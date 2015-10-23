using System.Windows.Forms;

namespace Quip {
  /// <summary>
  /// The application class.
  /// </summary>
  static class Program {
    /// <summary>
    /// The application's entry point.
    /// </summary>
    static void Main () {
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new QuipWindow());
    }
  }
}
