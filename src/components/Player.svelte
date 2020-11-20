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

  let secs = 0;
  $: if (meta !== null) {
    secs = Math.round(meta.length / meta.rate);
  }

  $: timestamp = () => {
    const elapsed = Math.round(cursor / meta.rate);
    const elpStr = `${Math.floor(elapsed / 60)}:${padSecs(elapsed % 60)}`;
    const totalStr = `${Math.floor(secs / 60)}:${padSecs(secs % 60)}`;
    return `${elpStr} / ${totalStr}`;
  };

  const padSecs = (secs: number) => {
    const str = secs.toString();
    return str.length > 1 ? str : `0${str}`
  };

  const skip = (e: MouseEvent) => {
    const el = e.currentTarget as HTMLDivElement;
    const rect = el.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const ratio = x / rect.width;
    const sample = Math.floor(meta.length * ratio);
    dispatch('skip', sample);
  };
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
    <div class='timeline' on:click={skip}>
      <div class='prg' style='width: {prgWidth}px'></div>
    </div>
    <div class='ut'>
      <div class='ts'>{timestamp()}</div>
      <div class='ctrls'>
        <div class='icon' on:click={() => dispatch('play')}>
          <Icon icon={playing ? faPause : faPlay} class='ctrl-btn play' />
        </div>
        <div class='icon' on:click={() => dispatch('stop')}>
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

<style lang='scss'>
  .empty {
    height: 105px;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 1.3em;
    color: rgb(255, 255, 255);
    transition-duration: .2s;

    & span {
      margin-left: 10px;
    }

    &:hover {
      cursor: pointer;
      color: rgb(55, 179, 243);
    }
  }

  .icon {
    line-height: 0px;
  }

  .player {
    display: flex;
    justify-content: center;
    flex-direction: column;
    width: 100%;
    padding: 0px 15px;
    height: 105px;
    box-sizing: border-box;
  }

  .timeline {
    height: 10px;
    background: rgb(43, 43, 46);
    position: relative;
    border-radius: 5px;
    overflow: hidden;
    flex-shrink: 0;
    cursor: pointer;
  }

  .prg {
    background: rgb(55, 179, 243);
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
  }

  .ctrls {
    display: flex;
    justify-content: center;
    font-size: 20px;
    color: rgb(162, 162, 167);
  }

  .ts {
    line-height: 1em;
    font-size: 15px;
    color: rgb(204, 204, 204);
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

    & span {
      margin-left: 5px;
      font-size: 11px;
      line-height: 1em;
    }

    &:hover {
      cursor: pointer;
      color: rgb(55, 179, 243);
    }
  }

  :global(.play) {
    margin-right: 10px;
  }

  :global(.ctrl-btn) {
    cursor: pointer;
    transition-duration: .3s;
  }

  :global(.ctrl-btn:hover) {
    color: rgb(55, 179, 243);
  }
</style>
