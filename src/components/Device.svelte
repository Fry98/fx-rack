<script lang="ts">
  import { faTimes } from '@fortawesome/free-solid-svg-icons';
  import { createEventDispatcher } from 'svelte';
  import Icon from 'fa-svelte';

  const dispatch = createEventDispatcher();
  export let name: string;
</script>

<div class='device'>
  <div class='header'>
    <div class='title'>{name}</div>
    <div class='close' on:click={() => dispatch('remove')}>
      <Icon icon={faTimes} />
    </div>
  </div>
  <main>
    <slot></slot>
  </main>
</div>

<style lang="scss">
  .device {
    margin: 7px 10px;
  }

  main {
    text-align: left;
    color: white;
  }

  .close {
    line-height: 0px;
    cursor: pointer;
    transition-duration: .3s;
    display: flex;
    align-items: center;

    &:hover {
      color: white;
    }
  }

  .header {
    color: rgb(160, 160, 160);
    display: flex;
    justify-content: space-between;
    font-size: 1.4em;
  }

  .title {
    color: white;
    font-weight: bold;
  }
</style>
