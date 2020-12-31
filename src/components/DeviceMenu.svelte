<script lang="ts">
  import { faTimes } from '@fortawesome/free-solid-svg-icons';
  import Icon from 'fa-svelte';
  import { createEventDispatcher } from 'svelte';
  import { DeviceType } from '../types';

  const dispatch = createEventDispatcher();
  export let open = false;

  const addFilter = () => {
    dispatch('device', {
      type: DeviceType.FILTER,
      cutoff: 0.4,
      slope: 30,
      hp: false
    });
    open = false;
  };

  const addReverb = () => {
    dispatch('device', {
      type: DeviceType.REVERB,
      delay: 100,
      decayFactor: 0.5,
      mix: 50
    });
    open = false;
  };

  const addDistortion = () => {
    dispatch('device', {
      type: DeviceType.DISTORTION,
      gain: 10,
      threshold: 10
    });
    open = false;
  };
</script>

<div class='menu' class:menu-hide={!open}>
  <div class='menu-ctn' on:click|self={() => open = false}>
    <div class='close-menu' on:click={() => open = false}>
      <Icon icon={faTimes} />
    </div>
    <button on:click={addFilter}>LP/HP Filter</button>
    <button on:click={addReverb}>Reverb</button>
    <button on:click={addDistortion}>Distortion</button>
  </div>
</div>
<div class='content' class:content-blur={open}>
  <slot></slot>
</div>

<style lang="scss">
  .menu {
    background: rgba(0, 0, 0, 0.37);
    position: fixed;
    top: 0px;
    left: 0px;
    width: 100%;
    height: 100%;
    z-index: 2;
    transition-duration: .4s;
  }

  .menu-ctn {
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
    transition-duration: .4s;
  }

  .menu-hide {
    opacity: 0;
    pointer-events: none;

    .menu-ctn {
      transform: scale(.9);
    }
  }

  .content-blur {
    filter: blur(2px);
  }

  .close-menu {
    position: absolute;
    top: 10px;
    right: 10px;
    color: rgb(162, 162, 167);
    font-size: 1.8em;
    cursor: pointer;
    transition-duration: .3s;

    &:hover {
      color: white;
    }
  }

  button {
    border: none;
    font-size: 1.4em;
    width: 200px;
    padding: 10px 0px;
    margin: 5px 0px;
    background: rgba(0, 0, 0, 0.26);
    color: rgb(214, 214, 214);
    border-radius: 7px;
    cursor: pointer;
    transition-duration: .3s;

    &:hover {
      color: white;
      background: rgba(0, 0, 0, 0.377);
    }
  }
</style>
