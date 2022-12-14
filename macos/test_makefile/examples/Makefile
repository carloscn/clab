.PHONY: clean distclean

op: bootgen key
	bash ./gen_boot_image.sh op
	@echo "make op finish"

ss: bootgen key
	bash ./gen_boot_image.sh ss
	@echo "make ss finish"

bootgen:
	 bash ./config.sh

key: ./keys/fsbl.nky ./keys/bitstream.nky ./keys/ppk.pub ./keys/spk.pub ./keys/psk.pem ./keys/ssk.pem
	echo "key pull finish"

./keys/fsbl.nky:
	bash -c "pushd ./keys && ./pull_xvault_keys.sh && popd"

./keys/bitstream.nky:
	bash -c "pushd ./keys && ./pull_xvault_keys.sh && popd"

./keys/spk.pub:
	bash -c "pushd ./keys && ./pull_xvault_keys.sh && popd"

./keys/ppk.pub:
	bash -c "pushd ./keys && ./pull_xvault_keys.sh && popd"

./keys/psk.pem:
	bash -c "pushd ./keys && ./pull_xvault_keys.sh && popd"

./keys/ssk.pem:
	bash -c "pushd ./keys && ./pull_xvault_keys.sh && popd"

distclean:
	rm -rf bootgen ./keys/*.pem ./keys/*.pub  ./keys/*.nky *.txt BOOT.BIN *.pem *.pub *.nky *.log

clean:
	rm -rf BOOT.BIN
