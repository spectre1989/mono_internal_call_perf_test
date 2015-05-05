using System;
using System.Runtime.CompilerServices;

namespace ManagedLibrary
{
	public class Widget
	{
		public int Number
		{
			get 
			{
				return get_Number_Internal(this.native_handle);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static int get_Number_Internal(IntPtr native_handle);

		public int Number2 {
			[MethodImpl(MethodImplOptions.InternalCall)]
			get;
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static Widget[] GetWidgets();

		private IntPtr native_handle = (IntPtr)0;
	}
}

