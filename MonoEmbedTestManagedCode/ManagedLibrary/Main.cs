using System;

namespace ManagedLibrary
{
	public static class Main
	{
		public static void EntryPoint()
		{
			Widget[] widgets = Widget.GetWidgets();
			const uint num_iterations = 1000;

			DateTime t = DateTime.Now;
			for(uint i = 0; i < num_iterations; ++i) 
			{
				int total = 0;

				foreach(Widget widget in widgets) 
				{
					total += widget.Number;
				}
			}
			TimeSpan span = DateTime.Now - t;
			Console.WriteLine(span.TotalMilliseconds);

			t = DateTime.Now;
			for(uint i = 0; i < num_iterations; ++i) 
			{
				int total = 0;

				foreach(Widget widget in widgets) 
				{
					total += widget.Number;
				}
			}
			span = DateTime.Now - t;
			Console.WriteLine(span.TotalMilliseconds);

			t = DateTime.Now;
			for(uint i = 0; i < num_iterations; ++i) 
			{
				int total = 0;

				foreach(Widget widget in widgets) 
				{
					total += widget.Number2;
				}
			}
			span = DateTime.Now - t;
			Console.WriteLine(span.TotalMilliseconds);
		}
	}
}

