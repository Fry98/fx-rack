<script lang="ts">
  import { faPlay, faPause, faStop, faFileUpload } from '@fortawesome/free-solid-svg-icons';
  import { createEventDispatcher } from 'svelte';
  import type { AudioMeta } from "../types";
  import Icon from 'fa-svelte';

  const dispatch = createEventDispatcher();
  export let meta: AudioMeta;
  export let filename: string;
  export let cursor: number;
  export let playing: boolean;

  let prgWidth = 0;
  $: if (meta !== null) {
    prgWidth = Math.round((cursor / meta.length) * 367);
  }
</script>

{#if meta}
  <div class='player'>
    <header>
      <div class='upload' on:click={() => dispatch('load')}>
        <Icon icon={faFileUpload} />
        <span>Select File</span>
      </div>
      <div>{filename}</div>
    </header>
    <div class='timeline'>
      <div class='prg' style='width: {prgWidth}px'></div>
    </div>
    <div class='ut'>
      <div class='ts'>0:00 / 0:17</div>
      <div class='ctrls'>
        <div on:click={() => dispatch('play')}>
          <Icon icon={playing ? faPause : faPlay} class='ctrl-btn play' />
        </div>
        <div on:click={() => dispatch('stop')}>
          <Icon icon={faStop} class='ctrl-btn' />
        </div>
      </div>
    </div>
  </div>
{:else}
  <div class='empty' on:click={() => dispatch('load')}>
    <Icon icon={faFileUpload} />
    <span>Select File</span>
  </div>
{/if}

<style>
  .empty {
    height: 113px;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 1.3em;
    color: rgb(255, 255, 255);
    transition-duration: .2s;
  }

  .empty:hover {
    cursor: pointer;
    color: rgb(100, 235, 143);
  }

  .empty span {
    margin-left: 10px;
  }

  .player {
    display: flex;
    justify-content: center;
    flex-direction: column;
    width: 100%;
    padding: 0px 15px;
    height: 113px;
    box-sizing: border-box;
  }

  .timeline {
    height: 10px;
    background: rgb(104, 104, 104);
    position: relative;
    border-radius: 5px;
    overflow: hidden;
    flex-shrink: 0;
  }

  .prg {
    background: rgb(0, 194, 65);
    position: absolute;
    top: 0px;
    left: 0px;
    height: 100%;
  }

  .ut {
    display: flex;
    justify-content: space-between;
    align-items: flex-start;
    margin: 0px 4px;
    margin-top: 10px;
    /* background: red; */
  }

  .ctrls {
    display: flex;
    justify-content: center;
    font-size: 20px;
    color: rgb(104, 104, 104);
  }

  .ts {
    line-height: 1em;
    font-size: 15px;
    color: rgb(104, 104, 104);
    font-weight: bold;
  }

  header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    color: rgb(255, 255, 255);
    margin-bottom: 10px;
  }

  .upload {
    display: flex;
    align-items: center;
    transition-duration: .2s;
  }

  .upload:hover {
    cursor: pointer;
    color: rgb(100, 235, 143);
  }

  .upload span {
    margin-left: 5px;
    font-size: 11px;
    line-height: 1em;
  }

  :global(.play) {
    margin-right: 10px;
  }

  :global(.ctrl-btn) {
    cursor: pointer;
    transition-duration: .3s;
  }

  :global(.ctrl-btn:hover) {
    color: rgb(173, 173, 173);
  }
</style>
