<script lang="ts">
	import { onDestroy, setContext } from 'svelte';
	import { writable } from 'svelte/store';
	import Knob from './components/Knob.svelte';
	import Player from './components/Player.svelte';
	import type { AudioMeta } from './types';
	const { ipcRenderer } = require('electron');
	const { dialog } = require('electron').remote;

	const precision = writable(false);
	setContext('precision', precision);

	let cursor: number = 0;
	let filename: string = null;
	let meta: AudioMeta = null;
	let playing: boolean = false;
	let vals = [10000, 20, 127];

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

	const skip = (e: any) => {
		cursor = e.detail;
		ipcRenderer.send('skip', e.detail);
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

	const mousedown = (e: Event) => e.preventDefault();
	const keydown = (e: KeyboardEvent) => {
		if (e.key === 'Shift') $precision = true;
	};
	const keyup = (e: KeyboardEvent) => {
		if (e.key === 'Shift') $precision = false;
	};

	document.addEventListener('mousedown', mousedown);
	document.addEventListener('keydown', keydown);
	document.addEventListener('keyup', keyup);

	onDestroy(() => {
		document.removeEventListener('mousedown', mousedown);
		document.removeEventListener('keydown', keydown);
		document.removeEventListener('keyup', keyup);
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
		on:skip={skip}
	/>
	<Knob
		name="Cutoff"
		lowerBound={50}
		upperBound={17000}
		bind:value={vals[0]}
	/>
	<Knob
		name="Q"
		lowerBound={0}
		upperBound={100}
		bind:value={vals[1]}
	/>
	<Knob
		name="Dry / Wet"
		lowerBound={0}
		upperBound={255}
		bind:value={vals[2]}
	/>
</main>

<style lang="scss">
	:global {
		@import './style/global.scss';
	}

	main {
		text-align: center;
	}
</style>
