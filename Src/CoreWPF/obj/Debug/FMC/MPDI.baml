   M S B A M L   `   `   `  ����  CoreWPF    CoreWPF.FMC.MPDI   5      \ XPresentationFramework, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      8x,http://schemas.microsoft.com/winfx/2006/xaml    5        � 	Resources  5       �� MergedDictionaries �� Source�� 5   
    5      �� 5      $! ../CommonStyleResource.xaml?�6%   %    5	      .��5      �    	GridStyle#��[  6    � RowDefinitions 5   
   �� 5      $��40 �6   �� 5      $��20 �6   �� 5      �� 5      $��20 �6   �� 5      .+�5   
   � $��0��6    � ColumnDefinitions 5      �� 5      �� 5      .+�5      �� -   5        $��m_AddMPDIList_button��5      $��0��61     ButtonStyle#��[ 6A   .��6|   
Add MPDI6�   �� 5      -   5        $��m_DelMPDIList_button��5      $��1��61   #��[ 6A   .��6|   Delete MPDI6�   5      �� 5   
   $��1��6     TextBlockStyle#��[ 6"   
MPDIList6T   �� 5   
   $��3��6   #��[ 6"   MPDI6Q   �� 5   
   -   5        $��m_MPDIList_listBox��5   "   $��2��6     UtilityListBoxStyle#��[ 6<    �� SelectionMode$ Single=�6�   �� 5   
   -   5        $��m_MPDI_listBox��5   "   $��4��6   #��[ 68   $ Extended=�6�    J� 	AllowDrop .  6�   5       5!      