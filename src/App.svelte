<script lang="ts">
	const { ipcRenderer } = require('electron');
	const { dialog } = require('electron').remote;

	const play = () => ipcRenderer.send('play');
	const stop = () => ipcRenderer.send('stop');
	const load = () => {
		dialog.showOpenDialog({
			properties: ['openFile'],
			filters: [
				{ name: 'Wave File', extensions: ['wav'] }
			]
		}).then((files: any) => {
			ipcRenderer.send('load', files.filePaths[0]);
		});
	};
</script>

<main>
	<button on:click={load}>load</button>
	<button on:click={play}>play</button>
	<button on:click={stop}>stop</button>
</main>

<style>
	main {
		text-align: center;
		padding: 1em;
		margin: 0 auto;
	}

	button {
		font-size: 1.05em;
		padding: 5px 12px;
	}
</style>
