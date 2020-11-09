<script lang="ts">
	import './components/Player.svelte';
	import Player from './components/Player.svelte';
	import type { AudioMeta } from './types';
	const { ipcRenderer } = require('electron');
	const { dialog } = require('electron').remote;

	let cursor: number = 0;
	let filename: string = null;
	let meta: AudioMeta = null;
	let playing: boolean = false;

	const play = () => {
		playing = !playing;
		if (playing) {
			if (cursor === meta.length) cursor = 0;
			ipcRenderer.send('play');
		} else {
			ipcRenderer.send('stop');
		}
	};

	const load = () => {
		dialog.showOpenDialog({
			properties: ['openFile'],
			filters: [
				{ name: 'Wave File', extensions: ['wav'] }
			]
		}).then((files: any) => {
			if (files.filePaths.length === 0) return;
			ipcRenderer.send('load', files.filePaths[0]);
			filename = files.filePaths[0].split('\\').pop();
		});
	};

	const stop = () => {
		playing = false;
		cursor = 0;
		ipcRenderer.send('reset');
	};

	ipcRenderer.on('cursorMove', (_, value: number) => {
		cursor = value;
		if (cursor >= meta.length) {
			playing = false;
		}
	});

	ipcRenderer.on('meta', (_, value: AudioMeta) => {
		meta = value;
		playing = false;
		cursor = 0;
	});
</script>

<main>
	<Player
		cursor={cursor}
		meta={meta}
		playing={playing}
		filename={filename}
		on:play={play}
		on:load={load}
		on:stop={stop}
	/>
</main>

<style>
	main {
		text-align: center;
	}
</style>
