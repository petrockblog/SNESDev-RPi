installservice:
	install -m 0755 SNESDev /etc/init.d
	update-rc.d SNESDev defaults
	/etc/init.d/SNESDev start

uninstallservice:
	/etc/init.d/SNESDev stop
	update-rc.d SNESDev remove
	-rm /etc/init.d/SNESDev
