### BEGIN OF DYNAMIC HOST-SPECIFIC INCLUSION 
exec_always --no-startup-id ~/.i3display

for_window [class=".kype-bin$"] exec --no-startu-id ~/.i3/toggle_layout 

exec --no-startup-id xsetroot -solid "#333333"
exec --no-startup-id nm-applet
### END OF DYNAMIC INCLUSION
############################################################

