<script lang="ts">
	import { onDestroy, setContext } from 'svelte';
	import { writable } from 'svelte/store';
	import Filter from './components/Filter.svelte';
	import Player from './components/Player.svelte';
	import type { Device, AudioMeta } from './types';
	import { DeviceType } from './types';
	const { ipcRenderer } = require('electron');
	const { dialog } = require('electron').remote;

	const precision = writable(false);
	setContext('precision', precision);

	let filename: string = null;
	let meta: AudioMeta = null;
	let playing: boolean = false;
	let deviceCount = 0;
	let cursor = 0;

	let devices: Device[] = [
		{
			id: 0,
			type: DeviceType.FILTER,
			hp: false,
			cutoff: 4000,
			slope: 20
		},
		{
			id: 1,
			type: DeviceType.FILTER,
			hp: false,
			cutoff: 4000,
			slope: 20
		}
	];

	const removeDevice = (idx: number) => {
		devices.splice(idx, 1);
		devices = devices;
	};

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
	<div class='divider'></div>
	{#each devices as device, i (device.id)}
		{#if device.type === DeviceType.FILTER}
			<Filter
				bind:cutoff={device.cutoff}
				bind:slope={device.slope}
				bind:type={device.hp}
				on:remove={() => removeDevice(i)}
			/>
		{/if}
		<div class='divider'></div>
	{/each}
</main>

<style lang="scss">
	:global {
		@import './style/global.scss';
	}

	.divider {
		border-top: 1px solid rgb(129, 129, 129);
		margin: 0px 20px;
	}

	main {
		text-align: center;
	}
</style>
