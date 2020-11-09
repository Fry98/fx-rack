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
			if (cursor < meta.length) {
				ipcRenderer.send('play')
			} else {
				setTimeout(() => playing = false, 200);
			}
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
			ipcRenderer.send('load', files.filePaths[0]);
			filename = files.filePaths[0].split('\\').pop();
		});
	};

	ipcRenderer.on('cursorMove', (_, value: number) => {
		cursor = value;
		if (cursor >= meta.length) {
			playing = false;
		}
	});

	ipcRenderer.on('meta', (_, value: AudioMeta) => {
		meta = value;
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
	/>
</main>

<style>
	main {
		text-align: center;
	}
</style>
