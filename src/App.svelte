<script lang="ts">
	import { onDestroy, setContext } from 'svelte';
	import { writable } from 'svelte/store';
	import Filter from './components/Filter.svelte';
	import Player from './components/Player.svelte';
	import Icon from 'fa-svelte';
	import type { Device, AudioMeta } from './types';
	import { DeviceType } from './types';
	import { faPlus } from '@fortawesome/free-solid-svg-icons';
	import DeviceMenu from './components/DeviceMenu.svelte';
	const { ipcRenderer } = require('electron');
	const { dialog } = require('electron').remote;

	const precision = writable(false);
	setContext('precision', precision);

	let filename: string = null;
	let meta: AudioMeta = null;
	let playing = false;
	let deviceCount = 0;
	let openMenu = false;
	let cursor = 0;
	let devices: Device[] = [];

	const removeDevice = (idx: number) => {
		devices.splice(idx, 1);
		devices = devices;
	};

	const addDevice = ({ detail }) => {
		devices.push({
			id: deviceCount++,
			...detail
		});
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

<DeviceMenu bind:open={openMenu} on:device={addDevice}>
	<main>
		<Player
			{cursor}
			{meta}
			{playing}
			{filename}
			on:play={play}
			on:load={load}
			on:stop={stop}
			on:skip={skip}
		/>
		{#each devices as device, i (device.id)}
			<div class='divider'></div>
			{#if device.type === DeviceType.FILTER}
				<Filter
					bind:cutoff={device.cutoff}
					bind:slope={device.slope}
					bind:type={device.hp}
					on:remove={() => removeDevice(i)}
				/>
			{/if}
		{/each}
		<div class='divider'></div>
		<div class='add-dev-wrap'>
			<div class='add-dev' on:click={() => openMenu = true}>
				<Icon icon={faPlus} />
				<div>Add Device</div>
			</div>
		</div>
	</main>
</DeviceMenu>

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

	.add-dev-wrap {
		display: flex;
		justify-content: center;
		margin-top: 15px;
	}

	.add-dev {
		font-size: 1.2em;
		line-height: 1em;
		border: 1px solid rgb(162, 162, 167);
		border-radius: 7px;
		color: rgb(162, 162, 167);
		padding: 7px;
		cursor: pointer;
		transition-duration: .3s;
		display: flex;
		margin-bottom: 20px;

		div {
			margin-left: 7px;
		}

		&:hover {
			color: white;
			border-color: white;
		}
	}
</style>
