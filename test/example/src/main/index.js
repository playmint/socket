import system from './system.js'
import path from 'path'
import fs from 'fs'

let counter = 0

async function main () {

  system.show({ index: 0 })

  const file = path.join(path.dirname(process.argv[1]), 'index.html')
  system.navigate({ index: 0, url: `file://${file}` })
  system.setTitle({ index: 0, value: 'Hello' })

  system.setMenu(`
    Operator:
      About Operator: _
      ---: _
      Preferences...: , + Command
      ---: _
      Hide: h
      Hide Others: h + Control, Command
      ---: _
      Quit: q + Command;

    Edit:
      Cut: x
      Copy: c
      Paste: v
      Delete: _
      Select All: a;

    Foo:
      Bazz: z + Command
      ---: _
      Quxx: e + ControlOrCommand, Option;

    Other:
      Another Test: t
      Beep: T + Command
  `)

  system.send({
    env: process.env,
    argv: process.argv
  })

  system.receive(async data => {
    let x = data

    return {
      received: data,
      counter: counter++
    }
  })

  setInterval(() => {
    counter++

    // send an odd sized message that can be validated
    // on the front end.
    const size = Math.floor(Math.random() * 1e3)
    const data = new Array(size).fill(0)

    //
    // we need to specify which window, the event name
    // that will be listening for this data, and the value
    // which can be plain old json data.
    //
    system.send({
      index: 0,
      event: 'data',
      value: {
        sending: data.join(''),
        counter,
        size
      }
    })
  }, 1024) // send at some interval

  process.on('beforeExit', () => {
    console.log('exiting')
  })
}

main()